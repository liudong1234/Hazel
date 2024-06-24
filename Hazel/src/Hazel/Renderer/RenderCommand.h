#pragma once

#include "RendererAPI.h"

namespace Hazel
{
<<<<<<< HEAD
    class RenderCommand
    {
    public:
        inline static void Init()
        {
            s_RendererAPI->Init();
        }
        inline static void SetViewPort(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
        {
            s_RendererAPI->SetViewPort(x, y, width, height);
        }

        inline static void SetClearColor(glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }
        inline static void SetClearColor(glm::vec4&& color)
        {
            s_RendererAPI->SetClearColor(color);
        }
        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }
        inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0)
        {
            s_RendererAPI->DrawIndexed(vertexArray, indexCount);
        }
    private:
        static RendererAPI* s_RendererAPI;
    };
=======
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(glm::vec4&& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void SetClearColor(glm::vec4& color)
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
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}