#include "hzpch.h"
//#include "VertexArray.h"
#include "Hazel/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel
{


    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported !");
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLVertexArray>();
        }
        return nullptr;
    }
}