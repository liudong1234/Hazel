#pragma once
#include "Hazel.h"
#include "Hazel/Renderer/Model.h"

class Sandbox3D:
	public Hazel::Layer
{
public:
	Sandbox3D();
	virtual ~Sandbox3D();

	void OnUpdate(Hazel::TimeStep ts) override;
	void OnAttach() override;
	void OnDetach() override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

private:
	Hazel::Ref<Hazel::Shader> shader;
	Hazel::FPSCameraController m_CameraController;
	std::shared_ptr<Hazel::Model> m_Model;
};