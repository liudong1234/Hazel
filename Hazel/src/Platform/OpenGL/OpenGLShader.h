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
        virtual void Bind() const override;
        virtual void UnBind() const override;
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

        void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
		void CompileOrGetOpenGLBinaries();
		void CompileOpenGLBinariesForAmd(GLenum& program, std::array<uint32_t, 2>& glShadersIDs);
		void CreateProgram();
		void CreateProgramForAmd();
		void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);
    private:
        uint32_t m_RendererID;
        std::string m_FilePath;
        std::string m_Name;

		std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
    };
}
