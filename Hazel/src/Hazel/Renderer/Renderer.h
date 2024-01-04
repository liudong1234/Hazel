#pragma once

#include "RenderCommand.h"

namespace Hazel
{
	class Renderer
	{
	public:
		static void BeginScene(); //场景开始

		static void EndScend(); //

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
	};
}