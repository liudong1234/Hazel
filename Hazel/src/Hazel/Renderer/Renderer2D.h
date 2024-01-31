#pragma once
#include "Hazel.h"

namespace Hazel
{
	class Renderer2D
	{
	public:

		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texutre);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texutre);



		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color, const float rotation);

	private:
		struct Render2DStorage
		{
			Ref<VertexArray> QuadVertexArray;
			Ref<Shader> TextureShader;
			Ref<Texture2D> WhiteTexture;
		};
		static Render2DStorage* s_Data;

		static void Draw();

	};
}