#pragma once

#include "RendererAPI.h"

namespace Hazel
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(glm::vec4&& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewPort(x, y, width, height);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}