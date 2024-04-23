#pragma once

#include <glm/glm.hpp>
#include "SceneCamera.h"

namespace Hazel
{
    struct TagComponent
    {
        std::string Tag{""};

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) :
            Tag(tag) {}

        operator std::string& () { return this->Tag; }//隐式强制类型转换
        operator const std::string& () const { return this->Tag; }
    };


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

    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true;//是否为主相机
        bool FixedAspectRatio = false;//是否保持纵横比

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };
}