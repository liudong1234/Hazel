#include "hzpch.h"
#include "Sandbox2D.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Debug/Instrumentor.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"),
    m_CameraController(1280.0f / 720.0f),
    m_Pos(glm::vec3(1.0f)),
    m_Color(glm::vec4(0.3f, 0.4f, 0.5f, 1.0f)),
    m_QuadPos(glm::vec3(0.0f, 0.0f, -0.1f)),
    m_QuadSize(glm::vec2(1.0f)),
    m_QuadAngle(0.0f)
{

}

Sandbox2D::~Sandbox2D()
{
    Hazel::Renderer2D::Shutdown();
}

void Sandbox2D::OnAttach()
{
    HZ_PROFILE_FUNCTION();

    this->quadTexture = Hazel::Texture2D::Create(std::string("Assets/Textures/1.png"));

}

void Sandbox2D::OnDetach()
{
    HZ_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Hazel::TimeStep ts)
{
    //InstrumentationTimer timer("sandbox2D Onupdate", [&](ProfileResult result) {this->m_ProfileResults.push_back(result); });
    HZ_PROFILE_FUNCTION();
    
    this->m_CameraController.OnUpdate(ts);
    Hazel::Renderer2D::ResetStatics();
    {
        HZ_PROFILE_SCOPE("Renderer Prop");
        Hazel::RenderCommand::Clear();
        Hazel::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0, 1.0f });
    }

    {
        HZ_PROFILE_SCOPE("Draw Quad");
        Hazel::Renderer2D::BeginScene(this->m_CameraController.GetCamera());
        Hazel::Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.1f, 0.2f, 0.3f, 1.0f });
        //Hazel::Renderer2D::DrawRotateQuad({ 1.0f, 0.8f }, this->m_QuadSize, this->m_QuadAngle, this->m_Color);
        Hazel::Renderer2D::DrawRotateQuad(this->m_QuadPos, this->m_QuadSize, this->m_QuadAngle, this->quadTexture, 2.0f, { 1.0f, 0.8f, 0.8f, 1.0f });
        //Hazel::Renderer2D::DrawQuad(this->m_QuadPos, this->m_QuadSize, this->quadTexture, 2.0f, {1.0f, 0.8f, 0.8f, 1.0f});
        Hazel::Renderer2D::EndScene();
    }

}

void Sandbox2D::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();
    ImGui::Begin("Test");
    ImGui::DragFloat3(u8"位置", &this->m_Pos.x, 0.1f);
    ImGui::DragFloat3("QuadPos", &this->m_QuadPos.x, 0.1f);
    ImGui::DragFloat2("QuadSize", &this->m_QuadSize.x, 0.1f);
    ImGui::DragFloat("QuadRotation", &this->m_QuadAngle, 1.0f, 0.0f, 360.0f);
    ImGui::ColorEdit4("Background", &this->m_Color[0]);

    auto stats = Hazel::Renderer2D::GetStats();
    ImGui::Text("QuadCalls:%d", stats.DrawCalls);
    ImGui::Text("QuadCount:%d", stats.QuadCounts);
    ImGui::Text("QuadIndexCount:%d", stats.GetQuadIndexCounts());
    ImGui::Text("QuadVertexCount:%d", stats.GetQuadVertexCounts());

    ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
    this->m_CameraController.OnEvent(e);
}
