#include "hzpch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

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

		s_Data->TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetUniformInt("tex", 0);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetUniformMat4("projection", camera.GetProjectionMatrix());
		s_Data->TextureShader->SetUniformMat4("view", camera.GetViewMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::Draw()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color)
	{
		s_Data->TextureShader->Bind();
		s_Data->WhiteTexture->Bind();


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		s_Data->TextureShader->SetUniformFloat4("color", color);
		s_Data->TextureShader->SetUniformMat4("transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
		s_Data->WhiteTexture->UnBind();
	}

<<<<<<< HEAD
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color, const float rotation)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetUniformFloat4("color", color);
		s_Data->WhiteTexture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		s_Data->TextureShader->SetUniformMat4("transform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
		s_Data->WhiteTexture->UnBind();
	}
=======

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetUniformFloat4("color", tintColor);
        s_Data->TextureShader->SetUniformFloat("u_TillingFactor", tillingFactor);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::scale(transform, { size.x, size.y, 1.0f });
        s_Data->TextureShader->SetUniformMat4("transform", transform);

        texture->Bind();
        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
        texture->UnBind();
    }

    void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4 color)
    {
        HZ_PROFILE_FUNCTION();

        DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, color);
    }

    void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4 color)
    {
        HZ_PROFILE_FUNCTION();
>>>>>>> 100debe (Improving 2D Rendering)

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

<<<<<<< HEAD
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetUniformFloat4("color", glm::vec4(1.0f));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetUniformMat4("transform", transform);

		texture->Bind();
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
		texture->UnBind();
	}
=======
    void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();
        
        DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetUniformFloat4("color", tintColor);
        s_Data->TextureShader->SetUniformFloat("u_TillingFactor", tillingFactor);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, { size.x, size.y, 1.0f });
        s_Data->TextureShader->SetUniformMat4("transform", transform);

        texture->Bind();
        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
        texture->UnBind();
    }
>>>>>>> 100debe (Improving 2D Rendering)

}