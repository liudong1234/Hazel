﻿#pragma once
#include "Hazel.h"

#include <string>

#include"imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Hazel/Core/TimeStep.h"


class Sandbox2D :
    public Hazel::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D();
    void OnUpdate(Hazel::TimeStep ts) override;
    void OnAttach();
    void OnDetach();
    void OnImGuiRender() override;
    void OnEvent(Hazel::Event& e) override;
private:
    Hazel::Ref<Hazel::Texture2D> quadTexture;
    Hazel::Ref<Hazel::SubTexture2D> subQuad;
    std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> s_TextureMap;

    Hazel::OrthographicCameraController m_CameraController;

    glm::vec3 m_Pos;

    glm::vec4 m_Color;

    glm::vec3 m_QuadPos;
    glm::vec2 m_QuadSize;
    float m_QuadAngle;
};