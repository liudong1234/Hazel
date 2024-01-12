#include "hzpch.h"
#include "OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

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



	OpenGLShader::OpenGLShader(std::string& filepath) :
		m_RendererID(0)
	{
		std::string source = ReadFile(filepath);
		this->m_OpenGLSource = PreProcess(source);
		this->CreateProgram();

	}

	OpenGLShader::OpenGLShader(std::string& vertexSrc, std::string& fragmentSrc):
		m_RendererID(0)
	{
		this->m_OpenGLSource[GL_VERTEX_SHADER] = vertexSrc;
		this->m_OpenGLSource[GL_FRAGMENT_SHADER] = fragmentSrc;
		this->CreateProgram();
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

	void OpenGLShader::SetUniformInt(const std::string& name, const int value)
	{
		int location = glGetUniformLocation(this->m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetUniformMatrix4(const std::string& name, const glm::mat4& matrix)
	{
		int location = glGetUniformLocation(this->m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(std::string& filepath)
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
			HZ_CORE_ASSERT(Hazel::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			HZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);

			shaderSource[Hazel::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSource;
	}
	
	void OpenGLShader::CreateProgram()
	{
		GLint program = glCreateProgram();

		std::vector<GLuint> shaderIDs(this->m_OpenGLSource.size());

		for (auto& kv : this->m_OpenGLSource)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;
			GLuint shader = glCreateShader(type);
			const GLchar* sourceCstr = source.c_str();
			
			glShaderSource(shader, 1, &sourceCstr, 0);
			glCompileShader(shader);


			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				HZ_CORE_ERROR("{0}", infoLog.data());
				HZ_CORE_ASSERT(false, "Vertex shader complication failure!");
				break;
			}
			glAttachShader(program, shader);
			shaderIDs.push_back(shader);
		}
		this->m_RendererID = program;
		glLinkProgram(this->m_RendererID);

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
			for (auto& id : shaderIDs)
			{
				glDeleteShader(id);
			}

			// Use the infoLog as you see fit.
			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Link shader program failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto& id : shaderIDs)
		{
			glDetachShader(this->m_RendererID, id);
		}
	}
}
