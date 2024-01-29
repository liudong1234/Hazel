#include "hzpch.h"
#include "Sandbox2D.h"
#include "Hazel/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() :
	Layer("Sandbox2D"),
	m_CameraController(1280.0f / 720.0f),
	m_Pos(glm::vec3(1.0f)),
	m_Color(glm::vec4(0.0f))
{

}

Sandbox2D::~Sandbox2D()
{
	Hazel::Renderer2D::Shutdown();
}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Hazel::TimeStep ts)
{
	this->m_CameraController.OnUpdate(ts);
	Hazel::RenderCommand::Clear();
	Hazel::RenderCommand::SetClearColor(this->m_Color);

	Hazel::Renderer2D::BeginScene(this->m_CameraController.GetCamera());
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_Color);
	Hazel::Renderer2D::EndScene();


}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Test");
	ImGui::DragFloat3(u8"位置", &this->m_Pos.x, 0.1f);
	ImGui::ColorPicker4("QuadColor", &this->m_Color[0]);
	ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	this->m_CameraController.OnEvent(e);
}
