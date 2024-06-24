#include "hzpch.h"
#include "Mesh.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "Renderer.h"

namespace Hazel
{
	/*Ref<Mesh> Mesh::Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Ref<Texture2D>> vTexture)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLMesh>(vertices, indices, vTexture);
		default:
			break;
		}
		HZ_CORE_ASSERT(false, "Unknown Renderer");
		return nullptr;
	}*/
}