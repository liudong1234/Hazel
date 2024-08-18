#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>

namespace Hazel
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0, OpenGL = 1
        };

    public:
        virtual void Init() = 0;
        virtual void SetViewPort(uint16_t x, uint16_t y, uint16_t width, uint16_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) = 0;
        virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
        inline static API GetAPI() { return s_API; }

		virtual void SetLineWidth(float width) = 0;
		static Scope<RendererAPI> Create();
    private:
        static API s_API;
    };
}

