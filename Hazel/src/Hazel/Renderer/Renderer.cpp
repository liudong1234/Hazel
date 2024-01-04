#include "hzpch.h"
#include "Renderer.h"

namespace Hazel
{
	void Renderer::BeginScene()
	{

	}

	void Renderer::EndScend()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}