#include "hzpch.h"
#include "OpenGLShader.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "Hazel/Core/Timer.h"
#include <filesystem>
namespace Hazel { 

    namespace Utils {

		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			HZ_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
				case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			HZ_CORE_ASSERT(false, "");
			return (shaderc_shader_kind)0;
		}

		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			HZ_CORE_ASSERT(false, "");
			return nullptr;
		}

		static const char* GetCacheDirectory()
		{
			// TODO: make sure the assets directory is valid
			return "Assets/cache/shader/opengl";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
			}
			HZ_CORE_ASSERT(false, "");
			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
			}
			HZ_CORE_ASSERT(false, "");
			return "";
		}

		static const bool IsAmdGpu()
		{
			const char* vendor = (char*)glGetString(GL_VENDOR);
			return strstr(vendor, "ATI") != nullptr;
		}

		static bool VerifyProgramLink(GLenum& program)
		{
			int isLinked = 0;
			glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
			if (isLinked == GL_FALSE)
			{
				int maxLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);
				glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

				glDeleteProgram(program);

				HZ_CORE_ERROR("{0}", infoLog.data());
				HZ_CORE_ASSERT(false, "[OpenGL] Shader link failure!");
				return false;
			}
			return true;
		}
	}

    OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_FilePath(filepath), m_RendererID(0)
    {
        HZ_PROFILE_FUNCTION();

        Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);

		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			if (Utils::IsAmdGpu()) 
				CreateProgramForAmd();
			else 
			{
				CompileOrGetOpenGLBinaries();
				CreateProgram();
			}
			HZ_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}

		// Extract name from filepath
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name), m_RendererID(0)
    {
        HZ_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderMap;
        shaderMap[GL_VERTEX_SHADER] = vertexSrc;
        shaderMap[GL_FRAGMENT_SHADER] = fragmentSrc;

        CompileOrGetVulkanBinaries(shaderMap);
		if (Utils::IsAmdGpu()) 
			CreateProgramForAmd();
		else 
		{
			CompileOrGetOpenGLBinaries();
			CreateProgram();
		}
    }

    OpenGLShader::~OpenGLShader()
    {
        HZ_PROFILE_FUNCTION();

        glDeleteProgram(this->m_RendererID);
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        HZ_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
        }
        else
            {
				HZ_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			HZ_CORE_ERROR("Could not open file '{0}'", filepath);
}

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        HZ_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);
            HZ_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			HZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

            shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }
        return shaderSources;

    }

    void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
            std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					HZ_CORE_ERROR(module.GetErrorMessage());
					HZ_CORE_ASSERT(false, "");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data]: shaderData)
			Reflect(stage, data);
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					HZ_CORE_ERROR(module.GetErrorMessage());
                HZ_CORE_ASSERT(false, "");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			HZ_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	void OpenGLShader::CreateProgramForAmd()
	{
		GLuint program = glCreateProgram();

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();
		std::filesystem::path shaderFilePath = m_FilePath;
		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + ".cached_opengl.pgr");
		std::ifstream in(cachedPath, std::ios::ate | std::ios::binary);

		if (in.is_open())
		{
			auto size = in.tellg();
			in.seekg(0);

			auto data = std::vector<char>(size);
			uint32_t format = 0;
			in.read((char*)&format, sizeof(uint32_t));
			in.read((char*)data.data(), size);
			glProgramBinary(program, format, data.data(), data.size());

			bool linked = Utils::VerifyProgramLink(program);

			if (!linked)
				return;
		}
		else
		{
			std::array<uint32_t, 2> glShadersIDs;
			CompileOpenGLBinariesForAmd(program, glShadersIDs);
			glLinkProgram(program);

			bool linked = Utils::VerifyProgramLink(program);

			if (linked)
			{
				// Save program data
				GLint formats = 0;
				glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
				HZ_CORE_ASSERT(formats > 0, "Driver does not support binary format");
				Utils::CreateCacheDirectoryIfNeeded();
				GLint length = 0;
				glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &length);
				auto shaderData = std::vector<char>(length);
				uint32_t format = 0;
				glGetProgramBinary(program, length, nullptr, &format, shaderData.data());
				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					out.write((char*)&format, sizeof(uint32_t));
					out.write(shaderData.data(), shaderData.size());
					out.flush();
					out.close();
				}
			}

			for (auto& id : glShadersIDs)
				glDetachShader(program, id);
		}

		m_RendererID = program;
	}

	void OpenGLShader::CompileOpenGLBinariesForAmd(GLenum& program, std::array<uint32_t, 2>& glShadersIDs)
	{
		int glShaderIDIndex = 0;
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			spirv_cross::CompilerGLSL glslCompiler(spirv);
			std::string source = glslCompiler.compile();

			uint32_t shader;

			shader = glCreateShader(stage);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				HZ_CORE_ERROR("{0}", infoLog.data());
				HZ_CORE_ASSERT(false, "[OpenGL] Shader compilation failure!");
				return;
			}
			glAttachShader(program, shader);
			glShadersIDs[glShaderIDIndex++] = shader;
		}
	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

            HZ_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
            HZ_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		HZ_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		HZ_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			HZ_CORE_TRACE("  {0}", resource.name);
			HZ_CORE_TRACE("    Size = {0}", bufferSize);
			HZ_CORE_TRACE("    Binding = {0}", binding);
			HZ_CORE_TRACE("    Members = {0}", memberCount);
		}
    }

    void OpenGLShader::Bind() const
    {
        HZ_PROFILE_FUNCTION();

        glUseProgram(this->m_RendererID);
    }

    void OpenGLShader::UnBind() const
    {
        HZ_PROFILE_FUNCTION();

        glUseProgram(0);
    }

    void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        HZ_PROFILE_FUNCTION();

        this->UpLoadUniformMat4(name, matrix);
    }

    void OpenGLShader::SetUniformFloat(const std::string& name, const float value)
    {
        HZ_PROFILE_FUNCTION();

        this->UpLoadUniformFloat(name, value);
    }

    void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        HZ_PROFILE_FUNCTION();

        this->UpLoadUniformFloat4(name, value);
    }

    void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        HZ_PROFILE_FUNCTION();

        this->UpLoadUniformFloat3(name, value);
    }

    void OpenGLShader::SetUniformInt(const std::string& name, int value)
    {
        HZ_PROFILE_FUNCTION();

        this->UpLoadUniformInt(name, value);
    }

    void OpenGLShader::SetUniformIntArray(const std::string& name, int* value, uint32_t count)
    {
        HZ_PROFILE_FUNCTION();
        this->UpLoadUniformIntArray(name, value, count);
    }

    void OpenGLShader::UpLoadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        int location = glGetUniformLocation(this->m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UpLoadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        int location = glGetUniformLocation(this->m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UpLoadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        int location = glGetUniformLocation(this->m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UpLoadUniformFloat(const std::string& name, const float value)
    {
        int location = glGetUniformLocation(this->m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UpLoadUniformInt(const std::string& name, const int value)
    {
        int location = glGetUniformLocation(this->m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UpLoadUniformIntArray(const std::string& name, int* value, uint32_t count)
    {
        int location = glGetUniformLocation(this->m_RendererID, name.c_str());
        glUniform1iv(location, count, value);
    }

}



/*
*       //create a empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

// Send the vertex shader source code to GL
// Note that std::string's .c_str is NULL character terminated.
        const GLchar* source = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            // Use the infoLog as you see fit.
            HZ_CORE_ERROR("{0}", infoLog.data());
            HZ_CORE_ASSERT(false, "Vertex shader complication failure!");
            return;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = (const GLchar*)fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            // Use the infoLog as you see fit.
            HZ_CORE_ERROR("{0}", infoLog.data());
            HZ_CORE_ASSERT(false, "fragment shader complication failure!");
            return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        this->m_RendererID = glCreateProgram();

        // Attach our shaders to our program
        glAttachShader(this->m_RendererID, vertexShader);
        glAttachShader(this->m_RendererID, fragmentShader);

        // Link our program
        glLinkProgram(this->m_RendererID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(this->m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(this->m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(this->m_RendererID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(this->m_RendererID);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // Use the infoLog as you see fit.
            HZ_CORE_ERROR("{0}", infoLog.data());
            HZ_CORE_ASSERT(false, "Link shader program failure!");
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(this->m_RendererID, vertexShader);
        glDetachShader(this->m_RendererID, fragmentShader);
*/
