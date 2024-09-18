#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel
{
	//RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
