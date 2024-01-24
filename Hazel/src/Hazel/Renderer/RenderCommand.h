#pragma once

#include "RendererAPI.h"

namespace Hazel
{
	class RenderCommand
	{
	public:
		inline void Init()
		{
			s_RendererAPI->Init();
		}
		inline static void SetViewPort(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
		{
			s_RendererAPI->SetViewPort(x, y, width, height);
		}

		inline static void SetClearColor(glm::vec4&& color)
		{
			s_RendererAPI->SetClearColor(color);
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