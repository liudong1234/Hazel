#pragma once
#include "Hazel/Renderer/Texture.h"
#include <glad/glad.h>

namespace Hazel
{
    class OpenGLTexture2D :
        public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string& filepath);
        OpenGLTexture2D(const char* filepath);
        OpenGLTexture2D(const uint32_t width, const uint32_t height);
        virtual ~OpenGLTexture2D();

        virtual void Bind(uint32_t slot = 0) override;
        virtual void UnBind() override;

        virtual uint32_t GetWidth() const override { return this->m_Width; }
        virtual uint32_t GetHeight() const override { return this->m_Height; }

        virtual void SetData(void* data, uint32_t size) override;
        virtual uint32_t GetRendererID() override { return this->m_RendererID; }
        
        virtual bool operator == (const Texture& other) const override 
        {
            return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
        }


    private:
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };
}
