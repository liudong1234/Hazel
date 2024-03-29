﻿#pragma once
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
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4 color);
        static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4 color);
        static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tillingFactor = 1.0f, const glm::vec4 & tintColor = glm::vec4(1.0f));
        static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

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