<<<<<<< HEAD
﻿#pragma once

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

        virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) override;
        
        virtual void SetUniformFloat(const std::string& name, const float value) override;
        virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;
        virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void SetUniformInt(const std::string& name, int value) override;
        virtual void SetUniformIntArray(const std::string& name, int* value, uint32_t count) override;

        void UpLoadUniformMat4(const std::string& name, const glm::mat4& matrix);
        void UpLoadUniformFloat4(const std::string& name, const glm::vec4& value);
        void OpenGLShader::UpLoadUniformInt(const std::string& name, const int value);
        void OpenGLShader::UpLoadUniformFloat3(const std::string& name, const glm::vec3& value);
        void OpenGLShader::UpLoadUniformFloat(const std::string& name, const float value);
        void OpenGLShader::UpLoadUniformIntArray(const std::string& name, int* value, uint32_t count);

        

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(std::unordered_map<GLenum, std::string>& sourceMap);

    private:
        uint32_t m_RendererID;
        std::string m_Name;
    };
=======
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
		OpenGLShader::OpenGLShader(const std::string& filepath);
		OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual const std::string& GetName() const override;

		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) override;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetUniformFloat1(const std::string& name, const float value) override;
		virtual void SetUniformInt(const std::string& name, const int value) override;
		virtual void SetuinformIntArray(const std::string& name, const int* value, const int count) override;


		void UpLoadUniformMatrix4(const std::string& name, const glm::mat4& matrix);
		void UpLoadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UpLoadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UpLoadUniformFloat1(const std::string& name, const float value);
		void UpLoadUniformInt(const std::string& name, const int value);
		void UpLoadUniformIntArray(const std::string& name, const int* value, const int count);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source);

		void Compile(std::unordered_map<GLenum, std::string> m_OpenGLSource);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}