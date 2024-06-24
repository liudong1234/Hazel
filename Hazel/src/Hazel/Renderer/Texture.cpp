<<<<<<< HEAD
﻿#include "hzpch.h"
#include "Renderer.h"

#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
=======
#include "hzpch.h"
#include "Renderer.h"

#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"


>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
namespace Hazel
{
	Ref<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
<<<<<<< HEAD
			case RendererAPI::API::None:
				HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported !");
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(width, height);
		}
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported !");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(filepath);
		}
		return nullptr;
	}
=======
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		default:
			break;
		}
		HZ_CORE_ASSERT(false, "Unknown Renderer");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(std::string filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(filepath);
		default:
			break;
		}
		HZ_CORE_ASSERT(false, "Unknown Renderer");
		return nullptr;
	}

>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}