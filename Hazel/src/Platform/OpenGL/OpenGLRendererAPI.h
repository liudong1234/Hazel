#pragma once
#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel
{
    class OpenGLRendererAPI :
        public RendererAPI
    {
    public:
        virtual void Init() override;
        virtual void SetViewPort(uint16_t x, uint16_t y, uint16_t width, uint16_t height) override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)  override;

		virtual void SetLineWidth(float width) override;

    private:

    };
}
