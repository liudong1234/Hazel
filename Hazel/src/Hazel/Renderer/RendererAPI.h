#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>

namespace Hazel
{
<<<<<<< HEAD
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
        inline static API GetAPI() { return s_API; }
    private:
        static API s_API;
    };
=======
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const uint32_t indexCount = 0) = 0;
		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}

