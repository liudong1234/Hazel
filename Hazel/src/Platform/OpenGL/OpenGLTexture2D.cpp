#include "hzpch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"

namespace Hazel
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
    {
        HZ_PROFILE_FUNCTION();

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr;
        {
            HZ_PROFILE_SCOPE("stbi_load OpenGLTexture2D::OpenGLTexture2D(const std::string&)");

            data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        }
        HZ_CORE_ASSERT(data, "Texture loads failly!");

        this->m_Height = height;
        this->m_Width = width;

        GLenum internalFormat = 0, dataFormat = 0;

        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        this->m_DataFormat = dataFormat;
        this->m_InternalFormat = internalFormat;

        HZ_CORE_ASSERT(internalFormat & dataFormat, "texuture format doesn't support");

        glCreateTextures(GL_TEXTURE_2D, 1, &this->m_RendererID);
        glTextureStorage2D(this->m_RendererID, 1, internalFormat, this->m_Width, this->m_Height);

        glTextureParameteri(this->m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(this->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureSubImage2D(this->m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

    }

    OpenGLTexture2D::OpenGLTexture2D(const char* filepath) :
        OpenGLTexture2D(std::string(filepath))
    {
        HZ_PROFILE_FUNCTION();

    }

    OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height) :
        m_Width(width),
        m_Height(height),
        m_InternalFormat(GL_RGBA8),
        m_DataFormat(GL_RGBA)
    {
        HZ_PROFILE_FUNCTION();

        glCreateTextures(GL_TEXTURE_2D, 1, &this->m_RendererID);
        glTextureStorage2D(this->m_RendererID, 1, this->m_InternalFormat, width, height);
        glTextureParameteri(this->m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(this->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(this->m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(this->m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &this->m_RendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot)
    {
        glBindTextureUnit(slot, this->m_RendererID);
    }

    void OpenGLTexture2D::UnBind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        uint32_t bbp = this->m_DataFormat == GL_RGBA ? 4 : 3;

        HZ_CORE_ASSERT(size == this->m_Width * this->m_Height * bbp, "Data must be entire texture!");

        glTextureSubImage2D(this->m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, this->m_DataFormat, GL_UNSIGNED_BYTE, data);

    }
}