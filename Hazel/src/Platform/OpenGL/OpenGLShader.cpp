#include "hzpch.h"
#include "OpenGLShader.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		HZ_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) :
		m_RendererID(0)
	{
		std::string shaderSource = this->ReadFile(filepath);
		auto& source = this->PreProcess(shaderSource);
		this->Compile(source);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? 0 : (filepath.size() - lastSlash);

		this->m_Name = filepath.substr(lastSlash, count);

	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) :
		m_RendererID(0),
		m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderMap;
		shaderMap[GL_VERTEX_SHADER] = vertexSrc;
		shaderMap[GL_FRAGMENT_SHADER] = fragmentSrc;

		this->Compile(shaderMap);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(this->m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
			HZ_CORE_ERROR("Could't not open file : {0}", filepath);
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specificed");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;

	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& source)
	{
		this->m_RendererID = glCreateProgram();

		//std::vector<GLint> vShader;
		//vShader.reserve(source.size());
		HZ_CORE_ASSERT(source.size() <= 2, "support 2 ");

		std::array<GLenum, 2> vShader;
		int index = 0;

		for (auto vk : source)
		{
			GLuint shader = glCreateShader(vk.first);
			const GLchar* shaderStr = vk.second.c_str();
			glShaderSource(shader, 1, &shaderStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				HZ_CORE_ERROR("{0}", infoLog.data());
				HZ_CORE_ASSERT(false, "Vertex shader complication failure!");
				return;
			}
			//vShader.push_back(shader);
			vShader[index++] = shader;
		}
		for (auto& shader : vShader)
		{
			glAttachShader(this->m_RendererID, shader);

		}
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
			for (auto& shader : vShader)
			{
				glDeleteShader(shader);
			}
			// Use the infoLog as you see fit.
			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Link shader program failure!");
			return;
		}
		// Always detach shaders after a successful link.
		for (auto& shader : vShader)
		{
			glDetachShader(this->m_RendererID, shader);
		}
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(this->m_RendererID);
	}

	void OpenGLShader::UnBind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		this->UpLoadUniformMat4(name, matrix);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		this->UpLoadUniformFloat4(name, value);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		this->UpLoadUniformFloat3(name, value);
	}

	void OpenGLShader::SetUniformInt(const std::string& name, int value)
	{
		this->UpLoadUniformInt(name, value);
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

	void OpenGLShader::UpLoadUniformInt(const std::string& name, const int value)
	{
		int location = glGetUniformLocation(this->m_RendererID, name.c_str());
		glUniform1i(location, value);
	}


}



/*
* 		//create a empty vertex shader handle
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