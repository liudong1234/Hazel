#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "OrthographicCamera.h"
namespace Hazel
{
	class Renderer
	{
	public:
		static void Init();

		static void OnResize(uint16_t width, uint16_t height);

		static void BeginScene(OrthographicCamera& camera); //场景开始

		static void EndScend(); //

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 viewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};
}