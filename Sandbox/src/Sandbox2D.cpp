#include "hzpch.h"
#include "Sandbox2D.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Debug/Instrumentor.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWDDWWWWWWWWWWWW"
"WWWWWWWWWDDDDWWWWWWWWWWW"
"WWWWWWWWDDDDDDWWWWWWWWWW"
"WWWWWWWDDDDDDDDWWWWWWWWW"
"WWWWWWDDDDWWDDDDWWWWWWWW"
"WWWWWDDDDDWWDDDDDWWWWWWW"
"WWWWWWDDDDDDDDDDWWWWWWWW"
"WWWWWWWDDDDDDDDWWWWWWWWW"
"WWWWWWWWDDDCDDWWWWWWWWWW"
"WWWWWWWWWDDDDWWWWWWWWWWW"
"WWWWWWWWWWDDWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"),
    m_CameraController(1280.0f / 720.0f),
    m_Pos(glm::vec3(1.0f)),
    m_Color(glm::vec4(0.3f, 0.4f, 0.5f, 1.0f)),
    m_QuadPos(glm::vec3(0.0f, 0.0f, 0.1f)),
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

    this->quadTexture = Hazel::Texture2D::Create(std::string("Assets/map/spritesheet/roguelikeSheet_magenta.png"));
    this->s_TextureMap['D'] = Hazel::SubTexture2D::CreateFromCoords(this->quadTexture, {0, 18}, {17, 17});
    this->s_TextureMap['W'] = Hazel::SubTexture2D::CreateFromCoords(this->quadTexture, {3, 26}, {17, 17});
    this->subQuad = Hazel::SubTexture2D::CreateFromCoords(this->quadTexture, { 0, 0 }, { 17, 17 });
    Hazel::FramebufferSpecification spec;
    spec.Width = Hazel::Application::Get().GetWindow().GetWidth();
    spec.Height = Hazel::Application::Get().GetWindow().GetHeight();
    this->m_Framebuffer = Hazel::Framebuffer::Create(spec);
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
        this->m_Framebuffer->Bind();
        Hazel::RenderCommand::Clear();
        Hazel::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0, 1.0f });
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;
        HZ_PROFILE_SCOPE("Draw Quad");
        Hazel::Renderer2D::BeginScene(this->m_CameraController.GetCamera());
        Hazel::Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.1f, 0.2f, 0.3f, 1.0f });
        //Hazel::Renderer2D::DrawRotateQuad({ 1.0f, 0.8f }, this->m_QuadSize, this->m_QuadAngle, this->m_Color);
        //Hazel::Renderer2D::DrawRotateQuad(this->m_QuadPos, this->m_QuadSize, rotation, this->quadTexture, 2.0f, { 1.0f, 0.8f, 0.8f, 1.0f });
        //Hazel::Renderer2D::DrawQuad(this->m_QuadPos, this->m_QuadSize, this->quadTexture, 2.0f, {1.0f, 0.8f, 0.8f, 1.0f});
        //Hazel::Renderer2D::DrawQuad({ -1.0f, -1.0f, 0.1f }, { 1.0f, 1.0f }, this->subQuad);
        
        int Height =  strlen(s_MapTiles) / s_MapWidth;

        for (int i = 0; i < Height; i++)
        {
            for (int j = 0; j < s_MapWidth; j++)
            {
                char ch = s_MapTiles[j + i * s_MapWidth];
                Hazel::Ref<Hazel::SubTexture2D> texture;
                if (this->s_TextureMap.find(ch) != this->s_TextureMap.end())
                    texture = this->s_TextureMap[ch];
                else
                    texture = this->subQuad;
                Hazel::Renderer2D::DrawQuad({ j - s_MapWidth / 2.0f, Height - i - Height / 2.0f, 0.1f }, { 1.0f, 1.0f }, texture);
            }
        }

        /*for (float x = -5.0f; x < 5.0f; x+=0.5f)
        {
            for (float y = -5.0f; y < 5.0f; y+=0.5f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 0.5f) / 10.0f, 1.0f };
                Hazel::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        }*/
        Hazel::Renderer2D::EndScene();
    }

    this->m_Framebuffer->UnBind();
}

void Sandbox2D::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();

    static bool p_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    ImGui::PopStyleVar();


    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            if (ImGui::MenuItem("Exit"))
            {
                Hazel::Application::Get().Close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();

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
        int width = this->m_Framebuffer->GetSpecification().Width / 2;
        int height = this->m_Framebuffer->GetSpecification().Height / 2;
        ImGui::Image((ImTextureID)this->m_Framebuffer->GetColorAttachmentID(), ImVec2(width, height), { 0, 1 }, { 1, 0 });

        ImGui::End();
    }


    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
    this->m_CameraController.OnEvent(e);
}
