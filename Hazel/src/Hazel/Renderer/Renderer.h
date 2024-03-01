#pragma once

#include "RenderCommand.h"
#include "OrthoGraphicCamera.h"
#include "Shader.h"

namespace Hazel
{
	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(const OrthoGraphicCamera& camera); //场景开始
		static void OnWindowResize(uint32_t width, uint32_t height);


		static void EndScend(); //

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:

		struct SceneData
		{
			glm::mat4 ProjectionMatrix;
			glm::mat4 ViewMatrix;
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};
}