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

        void UpLoadUniformMat4(const std::string& name, const glm::mat4& matrix);
        void UpLoadUniformFloat4(const std::string& name, const glm::vec4& value);
        void OpenGLShader::UpLoadUniformInt(const std::string& name, const int value);
        void OpenGLShader::UpLoadUniformFloat3(const std::string& name, const glm::vec3& value);
        void OpenGLShader::UpLoadUniformFloat(const std::string& name, const float value);


    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(std::unordered_map<GLenum, std::string>& sourceMap);

    private:
        uint32_t m_RendererID;
        std::string m_Name;
    };
}