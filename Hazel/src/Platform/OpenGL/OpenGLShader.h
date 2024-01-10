#pragma once
#include "Hazel/Renderer/Shader.h"

namespace Hazel
{
	class OpenGLShader:
		public Shader
	{
	public:
		OpenGLShader::OpenGLShader(std::string& vertexSrc, std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void Bind() override;
		virtual void UnBind() override;

		void SetUniformInt(const std::string& name, const int value);

		void SetUniformMatrix4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
	};
}