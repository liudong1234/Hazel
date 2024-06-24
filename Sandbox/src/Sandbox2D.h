<<<<<<< HEAD
ï»¿#pragma once
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
=======
#pragma once
#include "Hazel.h"

#include "Hazel/Renderer/Texture.h"


class Sandbox2D:
	public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D();

	void OnUpdate(Hazel::TimeStep ts) override;

	void OnAttach() override;
	void OnDetach() override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;
private:
	Hazel::OrthoGraphicCameraController m_CameraController;
	
	glm::vec4 m_Color;
	Hazel::Ref<Hazel::Texture2D> texture;

	glm::vec3 m_Pos;
	float rotation;//½Ç¶È
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
};