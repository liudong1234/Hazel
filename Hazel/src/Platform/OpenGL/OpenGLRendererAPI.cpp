#include "hzpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>
namespace Hazel
{
<<<<<<< HEAD
    void OpenGLRendererAPI::Init()
    {
        HZ_PROFILE_FUNCTION();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetViewPort(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
    {
        uint32_t count = indexCount == 0 ? vertexArray->GetIndexBuffers()->GetCount() : indexCount;
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }
=======
	void OpenGLRendererAPI::Init()
	{
		HZ_PROFILE_FUNCTION();

		//glEnable(GL_FRAMEBUFFER_SRGB);//gamma 校正处理
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount)
	{
		vertexArray->Bind();

		uint32_t count = indexCount == 0 ? vertexArray->GetIndexBuffers()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7

}
