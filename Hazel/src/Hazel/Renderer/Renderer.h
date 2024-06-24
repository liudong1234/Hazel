#pragma once

#include "RenderCommand.h"
<<<<<<< HEAD
#include "Shader.h"
#include "OrthographicCamera.h"
=======
#include "OrthoGraphicCamera.h"
#include "Shader.h"

>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
namespace Hazel
{
	class Renderer
	{
	public:
		static void Init();

<<<<<<< HEAD
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
=======
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
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
		};
		static SceneData* m_SceneData;
	};
}