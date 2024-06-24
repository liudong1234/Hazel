<<<<<<< HEAD
﻿#include "hzpch.h"
#include "Renderer2D.h"
#include "Hazel/Renderer/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;

        float TexIndex;
        float TillingFactor;
    };

    struct Render2DData
    {
        const static uint32_t MaxQuad = 1000;
        const static uint32_t MaxVertices = MaxQuad * 4;
        const static uint32_t MaxIndices = MaxQuad * 6;
        static const uint32_t MaxTextureSlots = 32;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        uint32_t TextrueSlotIndex;
        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

        glm::vec4 QuadVertexPosition[4];

        Renderer2D::Statistics Stats;
    };

    static Render2DData s_Data;

    void Renderer2D::Init()
    {
        HZ_PROFILE_FUNCTION();

        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(sizeof(QuadVertex) * s_Data.MaxVertices);
        BufferLayout layout;

        layout = {
            { "g_Position", ShaderDataType::Float3 },
            { "g_Color", ShaderDataType::Float4 },
            { "g_TexCoord", ShaderDataType::Float2},
            { "g_TexIndex", ShaderDataType::Float},
            { "g_TillingFactor", ShaderDataType::Float}
        };
        s_Data.QuadVertexBuffer->SetLayout(layout);
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            offset += 4;
        }

        Ref<IndexBuffer> m_QuadIndex = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(m_QuadIndex);
        delete[] quadIndices;

        s_Data.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformIntArray("tex", samplers, s_Data.MaxTextureSlots);

        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        //顶点位置信息
        s_Data.QuadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPosition[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    }

    void Renderer2D::Shutdown()
    {
        HZ_PROFILE_FUNCTION();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        HZ_PROFILE_FUNCTION();

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformMat4("projectionView", camera.GetViewProjectionMatrix());

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextrueSlotIndex = 1;
    }

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        HZ_PROFILE_FUNCTION();

        glm::mat4 proView = camera.GetProjection() * glm::inverse(transform);
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformMat4("projectionView", proView);

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextrueSlotIndex = 1;
    }

    void Renderer2D::EndScene()
    {
        HZ_PROFILE_FUNCTION();
        uint32_t datasize = (uint8_t*)(s_Data.QuadVertexBufferPtr) - (uint8_t*)(s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, datasize);

        Flush();
    }

    void Renderer2D::Flush()
    {
        for (uint32_t i = 0; i < s_Data.TextrueSlotIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer2D::FlushReset()
    {
        EndScene();
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextrueSlotIndex = 1;
    }

    void Renderer2D::DrawQuad(const glm::mat4 transform, const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION();
        constexpr size_t quadVertexCount = 4;
        const float textureIndex = 0.0f;
        const float tillingFactor = 1.0f;

        glm::vec2 texCoords[4] =
        {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}
        };

        if (s_Data.QuadIndexCount >= Render2DData::MaxIndices)
        {
            FlushReset();
        }

        for (uint32_t i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TillingFactor = tillingFactor;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCounts++;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, color);
    }

    void Renderer2D::DrawQuad(const glm::mat4 transform, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
        {
            FlushReset();
        }

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextrueSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextrueSlotIndex;
            s_Data.TextureSlots[s_Data.TextrueSlotIndex] = texture;
            s_Data.TextrueSlotIndex++;
        }
        size_t quadVertexCount = 4;
        glm::vec2 texCoords[4] =
        {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}
        };

        for (uint32_t i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
            s_Data.QuadVertexBufferPtr->Color = tintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TillingFactor = tillingFactor;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCounts++;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        DrawQuad(transform, texture, tillingFactor, tintColor);

    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tillingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
        {
            FlushReset();
        }

        float textureIndex = 0.0f;
        auto texture = subTexture->GetTexture();
        for (uint32_t i = 1; i < s_Data.TextrueSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextrueSlotIndex;
            s_Data.TextureSlots[s_Data.TextrueSlotIndex] = texture;
            s_Data.TextrueSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        const glm::vec2* texCoords = subTexture->GetTexCoords();

        for (uint32_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
            s_Data.QuadVertexBufferPtr->Color = tintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TillingFactor = tillingFactor;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCounts++;

    }


    void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION();

        DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, color);
    }

    void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
        {
            FlushReset();
        }
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(1.0f)) *
            glm::scale(transform, { size.x, size.y, 1.0f });

        float textureIndex = 0;
        float tillingFactor = 1.0f;
        glm::vec2 texCoords[4] =
        {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}
        };

        for (uint32_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TillingFactor = tillingFactor;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCounts++;
    }

    void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();

        DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
        {
            FlushReset();
        }
        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextrueSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextrueSlotIndex;
            s_Data.TextureSlots[s_Data.TextrueSlotIndex] = texture;
            s_Data.TextrueSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        glm::vec2 texCoords[4] =
        {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}
        };

        for (uint32_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
            s_Data.QuadVertexBufferPtr->Color = tintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TillingFactor = tillingFactor;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCounts++;
    }

    void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tillingFactor, const glm::vec4& tintColor)
    {
        DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
        {
            FlushReset();
        }
        float textureIndex = 0.0f;
        auto texture = subTexture->GetTexture();
        for (uint32_t i = 1; i < s_Data.TextrueSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextrueSlotIndex;
            s_Data.TextureSlots[s_Data.TextrueSlotIndex] = texture;
            s_Data.TextrueSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        const glm::vec2* texCoords = subTexture->GetTexCoords();

        for (uint32_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
            s_Data.QuadVertexBufferPtr->Color = tintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TillingFactor = tillingFactor;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCounts++;
    }



    void Renderer2D::ResetStatics()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }
    
    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }
=======
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

		//��������
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

>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}