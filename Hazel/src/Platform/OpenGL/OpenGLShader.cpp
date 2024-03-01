#include "hzpch.h"
#include "OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Hazel
{

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		HZ_PROFILE_FUNCTION();

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
		HZ_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		auto& m_OpenGLSource = PreProcess(source);
		this->Compile(m_OpenGLSource);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		this->m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc):
		m_RendererID(0),
		m_Name(name)
	{
		HZ_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> m_OpenGLSource;
		m_OpenGLSource[GL_VERTEX_SHADER] = vertexSrc;
		m_OpenGLSource[GL_FRAGMENT_SHADER] = fragmentSrc;
		this->Compile(m_OpenGLSource);
	}
	
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(this->m_RendererID);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(this->m_RendererID);
	}
	
	void OpenGLShader::UnBind()
	{
		glUseProgram(0);
	}

	const std::string& OpenGLShader::GetName() const
	{
		return this->m_Name;
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		this->UpLoadUniformMatrix4(name, matrix);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		this->UpLoadUniformFloat4(name, value);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		this->UpLoadUniformFloat3(name, value);
	}

	void OpenGLShader::SetUniformFloat1(const std::string& name, const float value)
	{
		this->UpLoadUniformFloat1(name, value);
	}

	void OpenGLShader::SetUniformInt(const std::string& name, const int value)
	{
		this->UpLoadUniformInt(name, value);
	}

	void OpenGLShader::SetuinformIntArray(const std::string& name, const int* value, const int count)
	{
		this->UpLoadUniformIntArray(name, value, count);
	}

	void OpenGLShader::UpLoadUniformInt(const std::string& name, const int value)
	{
		int location = glGetUniformLocation(this->m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UpLoadUniformIntArray(const std::string& name, const int* value, const int count)
	{
		int location = glGetUniformLocation(this->m_RendererID, name.c_str());
		glUniform1iv(location, count, value);
	}

	void OpenGLShader::UpLoadUniformMatrix4(const std::string& name, const glm::mat4& matrix)
	{
		int location = glGetUniformLocation(this->m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UpLoadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		int location = glGetUniformLocation(this->m_RendererID, name.c_str());
		glUniform4fv(location, 1, &value[0]);
	}

	void OpenGLShader::UpLoadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		int location = glGetUniformLocation(this->m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UpLoadUniformFloat1(const std::string& name, const float value)
	{
		int location = glGetUniformLocation(this->m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
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
		std::unordered_map<GLenum, std::string> shaderSource;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);//找到后返回在源代码中的位置 
		while (pos != std::string::npos)//循环条件是 pos 不等于 std::string::npos，即在源代码中找到 #type 字符串。
		{
			size_t eol = source.find_first_of("\r\n", pos);//找到`#type`后的换行符位置 eol
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			HZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);

			shaderSource[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSource;
	}
	
	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> m_OpenGLSource)
	{
		this->m_RendererID = glCreateProgram();

		//std::vector<GLint> vShader;
		//vShader.reserve(source.size());
		HZ_CORE_ASSERT(m_OpenGLSource.size() <= 2, "support 2 ");

		std::array<GLenum, 2> vShader;
		int index = 0;

		for (auto vk : m_OpenGLSource)
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
}
