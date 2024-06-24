#pragma once
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
}