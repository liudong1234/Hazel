#include "hzpch.h"
#include "Model.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLModel.h"
namespace Hazel
{
	std::shared_ptr<Model> Model::Create(const char* filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLModel>(filepath);
		default:
			break;
		}
		HZ_CORE_ASSERT(false, "Unknown Renderer");
		return nullptr;
	}
}
