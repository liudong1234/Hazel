#include "Sandbox3D.h"
#include "Hazel/Renderer/Renderer3D.h"

#include "imgui.h"

Sandbox3D::Sandbox3D():
	Hazel::Layer("sandbox3d"),
	m_CameraController(640.0f / 360.0f)
{
	Hazel::Renderer3D::Init("Assets/Models/Nahida/scene.gltf");
}

Sandbox3D::~Sandbox3D()
{
	Hazel::Renderer3D::Shutdown();
}

void Sandbox3D::OnUpdate(Hazel::TimeStep ts)
{
	Hazel::RenderCommand::Clear();
	Hazel::RenderCommand::SetClearColor({ 0.5f, 0.4f, 0.3f, 1.0f });
	this->m_CameraController.OnUpdate(ts);

	Hazel::Renderer3D::BeginScene(this->m_CameraController.GetCamera());

	//Hazel::Renderer3D::ShowModel();

	Hazel::Renderer::EndScend();
}

void Sandbox3D::OnAttach()
{
}

void Sandbox3D::OnDetach()
{
}

void Sandbox3D::OnImGuiRender()
{
	ImGui::Begin("Sandbox3D");
	glm::vec3 front = this->m_CameraController.GetCamera().GetPosition();
	ImGui::DragFloat3("pos", &front.x, 0.1f);
	ImGui::End();
}

void Sandbox3D::OnEvent(Hazel::Event& e)
{
	this->m_CameraController.OnEvent(e);
}
