#pragma once

#include "Hazel/Renderer/Shader.h"
#include <string>
namespace Hazel
{
	class OpenGLShader :
		public Shader
	{
	public:
		OpenGLShader(std::string& vertexSrc, std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void Bind() override;
		virtual void UnBind() override;

		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
		void SetUniformFloat4(const std::string& name, const glm::vec4& value);
	private:
		uint32_t m_RendererID;

	};
}