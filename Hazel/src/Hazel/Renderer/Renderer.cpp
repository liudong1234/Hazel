#include "hzpch.h"
#include "Renderer.h"

namespace Hazel
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
		m_SceneData->viewMatrix = camera.GetViewMatrix();
		m_SceneData->projectionMatrix = camera.GetProjectionMatrix();
	}

	void Renderer::EndScend()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();
		shader->SetUniformMat4("projection", m_SceneData->projectionMatrix);
		shader->SetUniformMat4("view", m_SceneData->viewMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}