#include "hzpch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
namespace Hazel
{
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

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLIndexBuffer>(indices, size);
            default:
                break;
        }
        HZ_CORE_ASSERT(false, "Unknown Renderer");
        return nullptr;
    }
}