﻿#include "hzpch.h"
#include "Hazel/Renderer/Renderer2D.h"

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/UniformBuffer.h"
#include "Hazel/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;

        float TexIndex;
        float TillingFactor;

		int EntityID;
    };

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;

		float Thickness;
		float Fade;

		int EntityID;
	};

    struct Render2DData
    {
        const static uint32_t MaxQuad = 1000;
        const static uint32_t MaxVertices = MaxQuad * 4;
        const static uint32_t MaxIndices = MaxQuad * 6;
        static const uint32_t MaxTextureSlots = 32;

		//Rect
        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> QuadShader;
        Ref<Texture2D> WhiteTexture;
        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;


		//Line
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;
		uint32_t LineVertexCount = 0;
		uint32_t LineWidth = 1;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		//Circle
		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;
		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;


        uint32_t TextrueSlotIndex = 1;
        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

        glm::vec4 QuadVertexPosition[4];

        Renderer2D::Statistics Stats;
    
		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

    static Render2DData s_Data;

    void Renderer2D::Init()
    {
        HZ_PROFILE_FUNCTION();

		//Rect
        s_Data.QuadVertexArray = VertexArray::Create();
        s_Data.QuadVertexBuffer = VertexBuffer::Create(sizeof(QuadVertex) * s_Data.MaxVertices);
        BufferLayout layout;
		layout = {
			{ "g_Position",			ShaderDataType::Float3 },
			{ "g_Color",			ShaderDataType::Float4 },
			{ "g_TexCoord",			ShaderDataType::Float2 },
			{ "g_TexIndex",			ShaderDataType::Float  },
			{ "g_TillingFactor",	ShaderDataType::Float  },
			{ "g_EntityID", 		ShaderDataType::Int		}
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
        Ref<IndexBuffer> quadIndex = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIndex);
        delete[] quadIndices;

		//Texture Setting
        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;
        s_Data.QuadShader = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;
        //顶点位置信息
        s_Data.QuadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPosition[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };


		//Line
		s_Data.LineVertexArray = VertexArray::Create();
		s_Data.LineVertexBuffer = VertexBuffer::Create(sizeof(LineVertex) * s_Data.MaxVertices);
		s_Data.LineVertexBuffer->SetLayout({
			{ "g_Position",			ShaderDataType::Float3 },
			{ "g_Color",			ShaderDataType::Float4 },
			{ "g_EntityID", 		ShaderDataType::Int		}
			});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];
		s_Data.LineShader = Shader::Create("assets/shaders/Renderer2D_Line.glsl");


		//Circle
		s_Data.CircleVertexArray = VertexArray::Create();
		s_Data.CircleVertexBuffer = VertexBuffer::Create(sizeof(CircleVertex) * s_Data.MaxVertices);
		s_Data.CircleVertexBuffer->SetLayout({
			{ "g_WorldPosition",	ShaderDataType::Float3	},
			{ "g_LocalPosition",	ShaderDataType::Float3	},
			{ "g_Color",	ShaderDataType::Float4	},
			{ "g_Thickness",ShaderDataType::Float	},
			{ "g_Fade",		ShaderDataType::Float	},
			{ "g_EntityID", ShaderDataType::Int		}
			});
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIndex);//use quad indexbuffer
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];
		s_Data.CircleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");


        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Render2DData::CameraData), 0);
    }

    void Renderer2D::Shutdown()
    {
        HZ_PROFILE_FUNCTION();
		delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        HZ_PROFILE_FUNCTION();
		
		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Render2DData::CameraData));

		StartBatch();
    }
    
	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        HZ_PROFILE_FUNCTION();

        s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Render2DData::CameraData));

		StartBatch();
    }
	
	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		HZ_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Render2DData::CameraData));

		StartBatch();
	}

    void Renderer2D::EndScene()
    {
        HZ_PROFILE_FUNCTION();

        Flush();
    }

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextrueSlotIndex = 1;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
	}

    void Renderer2D::Flush()
    {
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_Data.TextrueSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}
		
		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);
			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);
			s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}

    }

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

    void Renderer2D::FlushReset()
    {
        EndScene();
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextrueSlotIndex = 1;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

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

        if (s_Data.QuadIndexCount >= Render2DData::MaxIndices)
        {
            NextBatch();
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
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);

    }

    void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();

        DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tillingFactor, const glm::vec4& tintColor)
    {
        DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tillingFactor, tintColor);
    }
	
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tillingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount >= Render2DData::MaxIndices)
        {
            NextBatch();
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

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
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
            NextBatch();
        }

        for (uint32_t i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TillingFactor = tillingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCounts++;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor, int entityID)
    {
        HZ_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
        {
            NextBatch();
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
			if (s_Data.TextrueSlotIndex >= Render2DData::MaxTextureSlots)
				NextBatch();
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
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCounts++;
    }

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		HZ_PROFILE_FUNCTION();
		constexpr size_t circleVertexCount = 4;
		if (s_Data.CircleIndexCount >= Render2DData::MaxIndices)
		{
			NextBatch();
		}

		for (uint32_t i = 0; i < circleVertexCount; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPosition[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPosition[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityID = entityID;
			s_Data.CircleVertexBufferPtr++;
		}
		s_Data.CircleIndexCount += 6;

		s_Data.Stats.QuadCounts++;
	}

    void Renderer2D::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID)
    {
		s_Data.LineVertexBufferPtr->Position = start;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;
		
		s_Data.LineVertexBufferPtr->Position = end;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		//从左上角
		glm::vec3 p0 = { position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z };
		glm::vec3 p1 = { position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z };
		glm::vec3 p2 = { position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z };
		glm::vec3 p3 = { position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z };
		
		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 lineVertices[4];

		for (int i = 0; i < 4; i++)
		{
			lineVertices[i] = transform * s_Data.QuadVertexPosition[i];
		}
		DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}


	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Texture)
			DrawQuad(transform, src.Texture, src.TillingFactor, src.Color, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
	}

	void Renderer2D::SetLineWidth(float width)
	{
		RenderCommand::SetLineWidth(width);
	}

    void Renderer2D::ResetStatics()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }
    
    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }
}
