#include "hzpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Hazel
{
    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) :
        m_Spec(spec)
    {
        this->Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &this->m_RenderID);
        glDeleteTextures(1, &this->m_ColorAttachment);
        glDeleteTextures(1, &this->m_DepthAttachment);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->m_RenderID);
        glViewport(0, 0, this->m_Spec.Width, this->m_Spec.Height);
    }

    void OpenGLFramebuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(const uint32_t width, const uint32_t height)
    {
        if (width == 0 || height == 0)
        {
            HZ_CORE_WARN("Attempt to resize framebuffer to {0}, {1}", width, height);
            return;
        }
        this->m_Spec.Width = width;
        this->m_Spec.Height = height;
        this->Invalidate();
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (this->m_RenderID)
        {
            glDeleteFramebuffers(1, &this->m_RenderID);
            glDeleteTextures(1, &this->m_ColorAttachment);
            glDeleteTextures(1, &this->m_DepthAttachment);
        }

        glCreateFramebuffers(1, &this->m_RenderID);
        glBindFramebuffer(GL_FRAMEBUFFER, this->m_RenderID);


        glCreateTextures(GL_TEXTURE_2D, 1, &this->m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, this->m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->m_Spec.Width, this->m_Spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_ColorAttachment, 0);


        glCreateTextures(GL_TEXTURE_2D, 1, &this->m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, this->m_DepthAttachment);
        //glTextureStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, this->m_Spec.Width, this->m_Spec.Height);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->m_Spec.Width, this->m_Spec.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->m_DepthAttachment, 0);
    
        HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");


        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
}