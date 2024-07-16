#include "hzpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Hazel
{
	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils
	{
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}
		static void CreateTextures(bool multisampled, uint32_t* outId, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outId);
		}
		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}
		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisampled, format, width, height, GL_FALSE);
			else
			{
				glad_glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);

		}
		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisampled, format, width, height, 0);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					return true;
			}
			return false;
		}

		static GLenum HazelFBTextureFormatToGL(FramebufferTextureFormat& format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::RGBA8:
					return GL_RGBA8;
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					return GL_RED_INTEGER;
					break;
			}
			HZ_CORE_ASSERT(false, "texture format error");
			return 0;
		}
	}	
	

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) :
        m_Spec(spec)
    {
		for (auto spec : this->m_Spec.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				this->m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				this->m_DepthAttachmentSpecification = spec;
		}

        this->Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &this->m_RenderID);
        //glDeleteTextures(1, &this->m_ColorAttachment);
		glDeleteTextures(this->m_ColorAttachments.size(), this->m_ColorAttachments.data());
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

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		HZ_CORE_ASSERT(attachmentIndex < this->m_ColorAttachments.size(), "");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		HZ_CORE_ASSERT(attachmentIndex < this->m_ColorAttachments.size(), "");
		auto& spec = this->m_ColorAttachmentSpecifications[attachmentIndex];
		GLenum type = Utils::HazelFBTextureFormatToGL(spec.TextureFormat);
		glClearTexImage(this->m_ColorAttachments[attachmentIndex], 0, type, GL_INT, &value);
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
            //glDeleteTextures(1, &this->m_ColorAttachment);
			glDeleteTextures(this->m_ColorAttachments.size(), this->m_ColorAttachments.data());
            glDeleteTextures(1, &this->m_DepthAttachment);

			this->m_ColorAttachments.clear();
			this->m_DepthAttachment = 0;
        }

        glCreateFramebuffers(1, &this->m_RenderID);
        glBindFramebuffer(GL_FRAMEBUFFER, this->m_RenderID);

		//Attachments;
		bool multisampled = this->m_Spec.Samples > 1;

		if (this->m_ColorAttachmentSpecifications.size())
		{
			this->m_ColorAttachments.resize(this->m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisampled, this->m_ColorAttachments.data(), this->m_ColorAttachments.size());
			for (size_t i=0; i<this->m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisampled, this->m_ColorAttachments[i]);
				switch (this->m_ColorAttachmentSpecifications[i].TextureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(this->m_ColorAttachments[i], this->m_Spec.Samples, GL_RGBA8, GL_RGBA, this->m_Spec.Width, this->m_Spec.Height, i);
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						Utils::AttachColorTexture(this->m_ColorAttachments[i], this->m_Spec.Samples, GL_R32I, GL_RED_INTEGER, this->m_Spec.Width, this->m_Spec.Height, i);
						break;
					default:
						break;
				}
			}
		}

		if (this->m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisampled, &this->m_DepthAttachment, 1);
			Utils::BindTexture(multisampled, this->m_DepthAttachment);
			switch (this->m_DepthAttachmentSpecification.TextureFormat)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Utils::AttachDepthTexture(this->m_DepthAttachment, this->m_Spec.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, this->m_Spec.Width, this->m_Spec.Height);
					break;
				default:
					break;
			}
		}

		if (this->m_ColorAttachments.size() > 1)
		{
			HZ_CORE_ASSERT(this->m_ColorAttachments.size() <= 4, "");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(this->m_ColorAttachments.size(), buffers);
		}
		else if (this->m_ColorAttachments.empty())
		{
			//only depth_pass
			glDrawBuffer(GL_NONE);
		}


        //glCreateTextures(GL_TEXTURE_2D, 1, &this->m_ColorAttachment);
        //glBindTexture(GL_TEXTURE_2D, this->m_ColorAttachment);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->m_Spec.Width, this->m_Spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        //
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_ColorAttachment, 0);
        //glCreateTextures(GL_TEXTURE_2D, 1, &this->m_DepthAttachment);
        //glBindTexture(GL_TEXTURE_2D, this->m_DepthAttachment);
        ////glTextureStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, this->m_Spec.Width, this->m_Spec.Height);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->m_Spec.Width, this->m_Spec.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->m_DepthAttachment, 0);
    
        HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");


        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
}
