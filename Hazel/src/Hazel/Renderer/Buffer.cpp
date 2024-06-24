<<<<<<< HEAD
﻿#include "hzpch.h"
=======
﻿																														#include "hzpch.h"
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
namespace Hazel
{
<<<<<<< HEAD
    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLVertexBuffer>(vertices, size);
            default:
                break;
        }
        HZ_CORE_ASSERT(false, "Unknown Renderer");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLVertexBuffer>(size);
            default:
                break;
        }
        HZ_CORE_ASSERT(false, "Unknown Renderer");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLIndexBuffer>(indices, count);
            default:
                break;
        }
        HZ_CORE_ASSERT(false, "Unknown Renderer");
        return nullptr;
    }
=======
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
			default:
				break;
		}
		HZ_CORE_ASSERT(false, "Unknown Renderer");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		default:
			break;
		}
		HZ_CORE_ASSERT(false, "Unknown Renderer");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLIndexBuffer>(indices, count	);
			default:
				break;
		}
		HZ_CORE_ASSERT(false, "Unknown Renderer");
		return nullptr;
	}
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}