#pragma once

#include <glm/glm.hpp>

namespace Hazel
{
    struct TransformComponent
    {
        glm::mat4 Transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4 & transform) :
            Transform(transform) {}

        operator glm::mat4& () { return this->Transform; }//隐式强制类型转换
        operator const glm::mat4& () const { return this->Transform; }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color{ 1.0f , 1.0f, 1.0f, 1.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color) :
            Color(color) {}
    };
}