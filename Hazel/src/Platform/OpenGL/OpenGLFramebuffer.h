#pragma once
#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel
{
    class OpenGLFramebuffer :
        public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();


        virtual void Bind() override;
        virtual void UnBind() override;
        virtual uint32_t GetColorAttachmentID(uint32_t index = 0) override 
		{
			HZ_CORE_ASSERT(index < this->m_ColorAttachments.size(), "");
			return this->m_ColorAttachments[index];
		}
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

        virtual void Resize(const uint32_t width, const uint32_t height) override;
        virtual FramebufferSpecification& GetSpecification() override { return this->m_Spec; }
        virtual const FramebufferSpecification& GetSpecification() const override { return this->m_Spec; }
    private:
        void Invalidate();

        //uint32_t m_ColorAttachment, m_DepthAttachment;
        uint32_t m_RenderID;

        FramebufferSpecification m_Spec;
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
    };
}
