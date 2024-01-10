#include "hzpch.h"
#include "Renderer.h"

#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
namespace Hazel
{
	Texture2D* Texture2D::Create(std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported !");
			case RendererAPI::API::OpenGL:
				return new OpenGLTexture2D(filepath);
		}
		return nullptr;
	}
}