#include "hzpch.h"
#include "EditorLayer.h"
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

namespace Hazel
{
    EditorLayer::EditorLayer() :
        Layer("EditorLayer"),
        m_CameraController(1280.0f / 720.0f),
        m_Pos(glm::vec3(1.0f)),
        m_Color(glm::vec4(0.3f, 0.4f, 0.5f, 1.0f)),
        m_QuadPos(glm::vec3(0.0f, 0.0f, 0.1f)),
        m_QuadSize(glm::vec2(1.0f)),
        m_QuadAngle(0.0f),
        m_ViewportSize(glm::vec2(0.0f)),
        m_ViewportFocus(false),
        m_ViewportHover(false)
    {

    }

    EditorLayer::~EditorLayer()
    {
        Renderer2D::Shutdown();
    }

    void EditorLayer::OnAttach()
    {
        HZ_PROFILE_FUNCTION();

        m_ActiveScene = CreateRef<Scene>();
        auto square = m_ActiveScene->CreateEntity();
        m_ActiveScene->Reg().emplace<TransformComponent>(square, glm::mat4(1.0f));
        m_ActiveScene->Reg().emplace<SpriteRendererComponent>(square, glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
        m_SquareEntity = square;

        this->quadTexture = Texture2D::Create(std::string("Assets/map/spritesheet/roguelikeSheet_magenta.png"));
        this->s_TextureMap['D'] = SubTexture2D::CreateFromCoords(this->quadTexture, { 0, 18 }, { 17, 17 });
        this->s_TextureMap['W'] = SubTexture2D::CreateFromCoords(this->quadTexture, { 3, 26 }, { 17, 17 });
        this->subQuad = SubTexture2D::CreateFromCoords(this->quadTexture, { 0, 0 }, { 17, 17 });
        FramebufferSpecification spec;
        spec.Width = 1280/*Application::Get().GetWindow().GetWidth()*/;
        spec.Height = 720/*Application::Get().GetWindow().GetHeight()*/;
        this->m_Framebuffer = Framebuffer::Create(spec);
    }

    void EditorLayer::OnDetach()
    {
        HZ_PROFILE_FUNCTION();

    }

    void EditorLayer::OnUpdate(TimeStep ts)
    {
        //InstrumentationTimer timer("EditorLayer Onupdate", [&](ProfileResult result) {this->m_ProfileResults.push_back(result); });
        HZ_PROFILE_FUNCTION();


        if (this->m_ViewportFocus)
            this->m_CameraController.OnUpdate(ts);


        Renderer2D::ResetStatics();
        {
            HZ_PROFILE_SCOPE("Renderer Prop");
            this->m_Framebuffer->Bind();
            RenderCommand::Clear();
            RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0, 1.0f });
        }

        /*{
            static float rotation = 0.0f;
            rotation += ts * 50.0f;
            HZ_PROFILE_SCOPE("Draw Quad");
            Renderer2D::BeginScene(this->m_CameraController.GetCamera());
            Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.1f, 0.2f, 0.3f, 1.0f });

            size_t Height = strlen(s_MapTiles) / s_MapWidth;

            for (int i = 0; i < Height; i++)
            {
                for (int j = 0; j < s_MapWidth; j++)
                {
                    char ch = s_MapTiles[j + i * s_MapWidth];
                    Ref<SubTexture2D> texture;
                    if (this->s_TextureMap.find(ch) != this->s_TextureMap.end())
                        texture = this->s_TextureMap[ch];
                    else
                        texture = this->subQuad;
                    Renderer2D::DrawQuad({ j - s_MapWidth / 2.0f, Height - i - Height / 2.0f, 0.1f }, { 1.0f, 1.0f }, texture);
                }
            }
            Renderer2D::EndScene();
        }*/
        //update scene

        Renderer2D::BeginScene(this->m_CameraController.GetCamera());
        this->m_ActiveScene->OnUpdate(ts);
        Renderer2D::EndScene();

        this->m_Framebuffer->UnBind();
    }

    void EditorLayer::OnImGuiRender()
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
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                if (ImGui::MenuItem("Exit"))
                {
                    Application::Get().Close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();

            ImGui::Begin("Settings");
            ImGui::DragFloat3(u8"位置", &this->m_Pos.x, 0.1f);
            ImGui::DragFloat3("QuadPos", &this->m_QuadPos.x, 0.1f);
            ImGui::DragFloat2("QuadSize", &this->m_QuadSize.x, 0.1f);
            ImGui::DragFloat("QuadRotation", &this->m_QuadAngle, 1.0f, 0.0f, 360.0f);

            auto& sprite = this->m_ActiveScene->Reg().get<SpriteRendererComponent>(this->m_SquareEntity);
            ImGui::ColorEdit4("Background", &sprite.Color[0]);

            auto stats = Renderer2D::GetStats();
            ImGui::Text("QuadCalls:%d", stats.DrawCalls);
            ImGui::Text("QuadCount:%d", stats.QuadCounts);
            ImGui::Text("QuadIndexCount:%d", stats.GetQuadIndexCounts());
            ImGui::Text("QuadVertexCount:%d", stats.GetQuadVertexCounts());
            ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
            ImGui::Begin("viewport");
            ImVec2 viewPanelSize = ImGui::GetContentRegionAvail();
            
            this->m_ViewportFocus = ImGui::IsWindowFocused();
            this->m_ViewportHover = ImGui::IsWindowHovered();
            
            //HZ_INFO("{0}, {1}", this->m_ViewportFocus,this->m_ViewportHover);
            
            Application::Get().GetImGuiLayer()->SetBlockEvents(!this->m_ViewportFocus || !this->m_ViewportHover);

            if (this->m_ViewportSize != *((glm::vec2*)&viewPanelSize))
            {
                this->m_Framebuffer->Resize(viewPanelSize.x, viewPanelSize.y);
                this->m_ViewportSize = { viewPanelSize.x, viewPanelSize.y };
                this->m_CameraController.OnResize(viewPanelSize.x, viewPanelSize.y);
            }

            ImGui::Image((void*)this->m_Framebuffer->GetColorAttachmentID(), ImVec2(this->m_ViewportSize.x, this->m_ViewportSize.y), { 0, 1 }, { 1, 0 });
            ImGui::End();
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        this->m_CameraController.OnEvent(e);
    }
}