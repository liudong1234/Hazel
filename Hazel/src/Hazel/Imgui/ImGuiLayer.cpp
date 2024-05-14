#include "hzpch.h"
#include "ImGuiLayer.h"

#include "Hazel/Core/Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//暂时头文件
#include <glfw/glfw3.h>
//#include <glad/glad.h>
namespace Hazel
{
    ImGuiLayer::ImGuiLayer() :
        Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {

    }

    void ImGuiLayer::OnAttach()
    {
        HZ_PROFILE_FUNCTION();

        //setup dear imgui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //enable keyboard controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //enable gamepad controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; //enable docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; //enable Multi-viewport/platform windows
        //io.ConfigFlags |= ImGuiViewportFlags_NoTaskBarIcon;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;


        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        //setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        HZ_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        if (this->m_BlockImGuiEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            event.m_Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
            event.m_Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        }
    }

    void ImGuiLayer::Begin()
    {
        HZ_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        HZ_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        //rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }


    void ImGuiLayer::OnImGuiRender()
    {
        HZ_PROFILE_FUNCTION();

        //ImGui::Begin();
    }

}