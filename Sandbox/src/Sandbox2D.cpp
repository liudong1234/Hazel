#include "Sandbox2D.h"
#include "Hazel/Renderer/Renderer2D.h"

#include "imgui.h"

Sandbox2D::Sandbox2D() :
	Hazel::Layer("sandbox2d"),
	m_CameraController(1280.0f / 720.0f),
	m_Color(glm::vec4(1.0f)),
	m_Pos({ 0.0f, 0.0f, 1.0f }),
	rotation(0.0f)
{
	this->texture = Hazel::Texture2D::Create("Assets/Textures/container2.png");
}

Sandbox2D::~Sandbox2D()
{
	Hazel::Renderer2D::Shutdown();
}

void Sandbox2D::OnUpdate(Hazel::TimeStep ts)
{
	HZ_PROFILE_FUNCTION();

	Hazel::RenderCommand::Clear();
	Hazel::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });

	this->m_CameraController.OnUpdate(ts);

	Hazel::Renderer2D::BeginScene(this->m_CameraController.GetCamera());

	//Hazel::Renderer2D::DrawQuad(this->m_Pos, { 1.0f, 1.0f }, this->m_Color);

	Hazel::Renderer2D::DrawQuad(this->m_Pos, { 1.0f, 1.0f }, this->texture, rotation, 2);

	//Hazel::Renderer2D::DrawQuad({ -2.0f, -2.0f }, { 1.0f, 1.0f }, this->texture);

	Hazel::Renderer2D::EndScend();
}
void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("settings");

	ImGui::ColorEdit4(u8"ÑÕÉ«", &this->m_Color[0]);
	ImGui::DragFloat3(u8"Î»ÖÃ", &this->m_Pos.x, 0.5f);
	ImGui::DragFloat(u8"½Ç¶È", &this->rotation, 1.0f, 0.0f, 360.0f);
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	this->m_CameraController.OnEvent(e);
}
