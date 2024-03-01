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
};