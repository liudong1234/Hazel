#pragma once
#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel
{
<<<<<<< HEAD
    class OpenGLRendererAPI :
        public RendererAPI
    {
    public:
        virtual void Init() override;
        virtual void SetViewPort(uint16_t x, uint16_t y, uint16_t width, uint16_t height) override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) override;
    private:

    };
=======
	class OpenGLRendererAPI :
		public RendererAPI
	{
	public:
		virtual void Init() override;


		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount = 0) override;
	private:

	};
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}