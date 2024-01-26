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
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;
private:
	Hazel::Ref<Hazel::Shader> shader;

	Hazel::Ref<Hazel::VertexArray> quadVa;
	Hazel::Ref<Hazel::Shader> quadShader;

	Hazel::OrthoGraphicCameraController m_CameraController;

	Hazel::Ref<Hazel::Texture2D> texture;
};