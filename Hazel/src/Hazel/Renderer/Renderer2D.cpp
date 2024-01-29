#include "hzpch.h"
#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	Renderer2D::Render2DStorage* Renderer2D::s_Data = new Renderer2D::Render2DStorage;

	void Renderer2D::Init()
	{
		float quadVertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f
		};
		s_Data->QuadVertexArray.reset(VertexArray::Create());

		Ref<VertexBuffer> m_QuadBuffer;

		m_QuadBuffer.reset(VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
		BufferLayout layout;

		layout = {
			{ "g_Position", ShaderDataType::Float3 },
			{"g_Texture", ShaderDataType::Float2}
		};
		m_QuadBuffer->SetLayout(layout);
		s_Data->QuadVertexArray->AddVertexBuffer(m_QuadBuffer);

		uint32_t quadIndices[] = { 0, 1, 2, 0, 3, 2 };
		Ref<IndexBuffer> m_QuadIndex;
		m_QuadIndex.reset(IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(m_QuadIndex);

		s_Data->FlatColorShader = Shader::Create("Assets/Shaders/quad.glsl");

		s_Data->QuadTexture.reset(Texture2D::Create((std::string)"Assets/Textures/1.png"));

		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->SetUniformInt("tex", 0);

	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->SetUniformMat4("projection", camera.GetProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->SetUniformMat4("view", camera.GetViewMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->SetUniformMat4("transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		//std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->SetUniformFloat4("color", color);
		s_Data->QuadVertexArray->Bind();
		s_Data->QuadTexture->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}