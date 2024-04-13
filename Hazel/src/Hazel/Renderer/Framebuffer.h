#pragma once

namespace Hazel
{
    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;
    };

    class Framebuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual uint32_t GetColorAttachmentID() = 0;

        virtual FramebufferSpecification& GetSpecification() = 0;
        virtual const FramebufferSpecification& GetSpecification() const = 0;
        virtual void Resize(const uint32_t width, const uint32_t height) = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
}