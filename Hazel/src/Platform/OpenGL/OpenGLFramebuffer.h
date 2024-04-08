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
        virtual uint32_t GetColorAttachmentID() override { return this->m_ColorAttachment; }

        virtual FramebufferSpecification& GetSpecification() override { return this->m_Spec; }
        virtual const FramebufferSpecification& GetSpecification() const override { return this->m_Spec; }
    private:
        void Invalidate();

        uint32_t m_ColorAttachment;
        uint32_t m_DepthAttachment;
        uint32_t m_RenderID;

        FramebufferSpecification m_Spec;

    };
}