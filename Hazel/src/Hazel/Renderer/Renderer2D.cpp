#include "hzpch.h"
#include "Renderer2D.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Hazel
{

	Renderer2D::SceneData Renderer2D::m_SceneData;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

		m_SceneData.QuadVertexArray = VertexArray::Create();
		m_SceneData.QuadVertexBuffer = VertexBuffer::Create(m_SceneData.MaxVertices * sizeof(QuadVertex));
		
		m_SceneData.QuadVertexBuffer->SetLayout
		(
			{
				{ "g_Position", ShaderDataType::Float3 },
				{ "g_Color", ShaderDataType::Float4},
				{"g_TexCoord", ShaderDataType::Float2},
				{"g_TexIndex", ShaderDataType::Float},
				{"g_TilFactor", ShaderDataType::Float}
			}
		);
		m_SceneData.QuadVertexArray->AddVertexBuffer(m_SceneData.QuadVertexBuffer);

		m_SceneData.QuadVertexBufferBase = new QuadVertex[m_SceneData.MaxVertices];

		//Éú³ÉË÷Òý
		uint32_t* quadIndices = new uint32_t[m_SceneData.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < m_SceneData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> m_QuadIndex;
		m_QuadIndex = IndexBuffer::Create(quadIndices, m_SceneData.MaxIndices);
		m_SceneData.QuadVertexArray->SetIndexBuffer(m_QuadIndex);
		delete[] quadIndices;


		m_SceneData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexture = 0xffffffff;
		m_SceneData.WhiteTexture->SetData(&whiteTexture, sizeof(uint32_t));

		int32_t samplers[m_SceneData.MaxTextureSlots];
		for (uint32_t i = 0; i < m_SceneData.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		m_SceneData.QuadShader = Shader::Create("Assets/Shaders/quad.glsl");
		m_SceneData.QuadShader->Bind();
		m_SceneData.QuadShader->SetuinformIntArray("v_Textures", samplers, m_SceneData.MaxTextureSlots);

		m_SceneData.TextureSlots[0] = m_SceneData.WhiteTexture;

		m_SceneData.QuadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_SceneData.QuadVertexPosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		m_SceneData.QuadVertexPosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		m_SceneData.QuadVertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	}

	void Renderer2D::BeginScene(const OrthoGraphicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();
		m_SceneData.QuadShader->SetUniformMat4("projection", camera.GetProjectionMatrix());
		m_SceneData.QuadShader->SetUniformMat4("view", camera.GetViewMatrix());
		
		m_SceneData.QuadIndexCount = 0;
		m_SceneData.QuadVertexBufferPtr = m_SceneData.QuadVertexBufferBase;
		m_SceneData.TextureSlotIndex = 1;
	}


	void Renderer2D::EndScend()
	{
		HZ_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::Flush()
	{
		HZ_PROFILE_FUNCTION();

		uint32_t datasize = (uint8_t*)m_SceneData.QuadVertexBufferPtr - (uint8_t*)m_SceneData.QuadVertexBufferBase;
		m_SceneData.QuadVertexBuffer->SetData(m_SceneData.QuadVertexBufferBase, datasize);

		for (uint32_t i=0; i<m_SceneData.TextureSlotIndex; i++)
		{
			m_SceneData.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(m_SceneData.QuadVertexArray, m_SceneData.QuadIndexCount);
	}

	void Renderer2D::Shutdown()
	{
		HZ_PROFILE_FUNCTION();

		delete[] m_SceneData.QuadVertexBufferBase;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		float textureIndex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		glm::vec2 textureIndices[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f }, {0.0f, 1.0f} };

		for (int i = 0; i < 4; i++)
		{
			m_SceneData.QuadVertexBufferPtr->Position = transform * m_SceneData.QuadVertexPosition[i];
			m_SceneData.QuadVertexBufferPtr->Color = color;
			m_SceneData.QuadVertexBufferPtr->TexCoord = textureIndices[i];
			m_SceneData.QuadVertexBufferPtr->TextureIndex = textureIndex;
			m_SceneData.QuadVertexBufferPtr++;
		}
		m_SceneData.QuadIndexCount += 6;
	}


	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation, const float tilFactor)
	{
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		
		float textureIndex = 0.0f;
		for(uint32_t i = 1; i < m_SceneData.TextureSlotIndex; i++)
		{
			if (*m_SceneData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)m_SceneData.TextureSlotIndex;
			m_SceneData.TextureSlots[m_SceneData.TextureSlotIndex] = texture;
			m_SceneData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		glm::vec2 textureIndices[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f }, {0.0f, 1.0f} };

		for (int i = 0; i < 4; i++)
		{
			m_SceneData.QuadVertexBufferPtr->Position = transform * m_SceneData.QuadVertexPosition[i];
			m_SceneData.QuadVertexBufferPtr->Color = color;
			m_SceneData.QuadVertexBufferPtr->TexCoord = textureIndices[i];
			m_SceneData.QuadVertexBufferPtr->TextureIndex = textureIndex;
			m_SceneData.QuadVertexBufferPtr->TilFactor = tilFactor;
			m_SceneData.QuadVertexBufferPtr++;
		}
		m_SceneData.QuadIndexCount += 6;


		/*m_SceneData.QuadVertexBufferPtr->Position = position;
		m_SceneData.QuadVertexBufferPtr->Color = color;
		m_SceneData.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		m_SceneData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		m_SceneData.QuadVertexBufferPtr->TilFactor = tilFactor;
		m_SceneData.QuadVertexBufferPtr++;

		m_SceneData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		m_SceneData.QuadVertexBufferPtr->Color = color;
		m_SceneData.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		m_SceneData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		m_SceneData.QuadVertexBufferPtr->TilFactor = tilFactor;
		m_SceneData.QuadVertexBufferPtr++;

		m_SceneData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		m_SceneData.QuadVertexBufferPtr->Color = color;
		m_SceneData.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		m_SceneData.QuadVertexBufferPtr->TilFactor = tilFactor;
		m_SceneData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		m_SceneData.QuadVertexBufferPtr++;

		m_SceneData.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		m_SceneData.QuadVertexBufferPtr->Color = color;
		m_SceneData.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		m_SceneData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		m_SceneData.QuadVertexBufferPtr->TilFactor = tilFactor;
		m_SceneData.QuadVertexBufferPtr++;*/

		m_SceneData.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation, const float tilFactor)
	{
		HZ_PROFILE_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, rotation, tilFactor);
	}

}