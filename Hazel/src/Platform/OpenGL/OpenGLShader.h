#pragma once
#include "Hazel/Renderer/Shader.h"


#include <unordered_map>

typedef unsigned int GLenum;
namespace Hazel
{

	class OpenGLShader:
		public Shader
	{
	public:
		OpenGLShader::OpenGLShader(std::string& filepath);
		OpenGLShader::OpenGLShader(std::string& vertexSrc, std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void Bind() override;
		virtual void UnBind() override;

		void SetUniformInt(const std::string& name, const int value);

		void SetUniformMatrix4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(std::string& filepath);
		std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source);

		void CreateProgram();
	private:
		uint32_t m_RendererID;
		std::unordered_map<GLenum, std::string> m_OpenGLSource;
	};
}