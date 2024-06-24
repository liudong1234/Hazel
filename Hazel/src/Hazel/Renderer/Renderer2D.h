<<<<<<< HEAD
ï»¿#pragma once
#include "Hazel.h"

namespace Hazel
{
    class Renderer2D
    {
    public:

        static void Init();
        static void Shutdown();

        static void Flush();
        static void BeginScene(const OrthographicCamera& camera);
        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void EndScene();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tillingFactor = 1.0f, const glm::vec4 & tintColor = glm::vec4(1.0f));
        static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));


        static void DrawQuad(const glm::mat4 transfrom, const glm::vec4& color);
        static void DrawQuad(const glm::mat4 transfrom, const Ref<Texture2D>& texture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        

        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCounts = 0;

            uint32_t GetQuadIndexCounts() { return QuadCounts * 6; }
            uint32_t GetQuadVertexCounts() { return QuadCounts * 4; }
        };
        static Statistics GetStats();
        static void ResetStatics();

    private:
        static void FlushReset();
    };
=======
#pragma once
#include "Hazel.h"

#include "Texture.h"

namespace Hazel
{
	class Renderer2D
	{
	public:
		static void Init();
		static void BeginScene(const OrthoGraphicCamera& camera); //³¡¾°¿ªÊ¼
		//static void OnWindowResize(uint32_t width, uint32_t height);

		static void EndScend(); //
		static void Flush();

		static void Shutdown();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation = 0.0f, const float tilFactor = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation = 0.0f, const float tilFactor = 1.0f);


	private:
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TextureIndex;
			float TilFactor;
			//index and so on
		};


		struct SceneData
		{
			//×î´ó¾ØÐÎÊýÁ¿
			const uint32_t MaxQuads = 10000;
			//×î¶à¶¥µãÊý
			const uint32_t MaxVertices = MaxQuads * 4;
			//×î¶àË÷ÒýÊý
			const uint32_t MaxIndices = MaxQuads * 6;
			//ÎÆÀí²å²ÛÊý
			static const uint32_t MaxTextureSlots = 32;

			Ref<Texture2D> WhiteTexture;//°×É«ÎÆÀí
			std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
			//0ÎªÄ¬ÈÏÎÆÀí mWhiteTexture;
			uint32_t TextureSlotIndex = 1; 

			//¾ØÐÎ
			Ref<VertexArray> QuadVertexArray;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<Shader> QuadShader;
			QuadVertex* QuadVertexBufferBase = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;
			uint32_t QuadIndexCount = 0;
			glm::vec4 QuadVertexPosition[4];

			//Èý½ÇÐÎ
			Ref<VertexArray> TriVertexArray;
			Ref<VertexBuffer> TriVertexBuffer;
			Ref<Shader> TriShader;
			QuadVertex* TriVertexBufferBase = nullptr;
			QuadVertex* TriVertexBufferPtr = nullptr;
			uint32_t TriIndexCount = 0;
			glm::vec4 TriVertexPosition[4];

		};
		static SceneData m_SceneData;
	};

>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}