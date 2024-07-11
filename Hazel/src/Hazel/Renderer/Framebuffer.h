#pragma once

namespace Hazel
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		//color
		RGBA8,
		RED_INTEGER,

		//Depth/stencil
		DEPTH24STENCIL8,

		//Default
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) 
			:TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments) 
			:Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};


    struct FramebufferSpecification
    {
        uint32_t Width, Height;
		FramebufferAttachmentSpecification Attachments;
        uint32_t Samples = 1;
    };

    class Framebuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual uint32_t GetColorAttachmentID(uint32_t index = 0) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

        virtual FramebufferSpecification& GetSpecification() = 0;
        virtual const FramebufferSpecification& GetSpecification() const = 0;
        virtual void Resize(const uint32_t width, const uint32_t height) = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
}
