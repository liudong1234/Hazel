#pragma once
#include "Hazel.h"

#include "Texture.h"

namespace Hazel
{
	class Renderer2D
	{
	public:
		static void Init();
		static void BeginScene(const OrthoGraphicCamera& camera); //场景开始
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
			//最大矩形数量
			const uint32_t MaxQuads = 10000;
			//最多顶点数
			const uint32_t MaxVertices = MaxQuads * 4;
			//最多索引数
			const uint32_t MaxIndices = MaxQuads * 6;
			//纹理插槽数
			static const uint32_t MaxTextureSlots = 32;

			Ref<Texture2D> WhiteTexture;//白色纹理
			std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
			//0为默认纹理 mWhiteTexture;
			uint32_t TextureSlotIndex = 1; 

			//矩形
			Ref<VertexArray> QuadVertexArray;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<Shader> QuadShader;
			QuadVertex* QuadVertexBufferBase = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;
			uint32_t QuadIndexCount = 0;
			glm::vec4 QuadVertexPosition[4];

			//三角形
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

}