#pragma once

#include "Hazel/Renderer/Shader.h"
#include <string>
#include <unordered_map>


namespace Hazel
{
	typedef unsigned int GLenum;

	class OpenGLShader :
		public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual const std::string& GetName() const override { return this->m_Name; }

		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
		void SetUniformFloat4(const std::string& name, const glm::vec4& value);
		void OpenGLShader::SetUniformInt(const std::string& name, const int value);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& sourceMap);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}