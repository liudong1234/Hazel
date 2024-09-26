#include "hzpch.h"
#include "Application.h"
#include "Input.h"
#include "Hazel/Renderer/Renderer.h"
#include <GLFW/glfw3.h>

namespace Hazel
{
    #define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;


    Application::Application(const ApplcationSpecification& specification)
		: m_Specification(specification),
        m_LastTime(0.0f),
        is_Minimum(false)
    {
        HZ_PROFILE_FUNCTION();

        HZ_CORE_ASSERT(!s_Instance, "application already exist! ");
        s_Instance = this;
		if (!specification.WorkingDir.empty())
			std::filesystem::current_path(specification.WorkingDir);

        this->m_Windnow = Scope<Window>(Window::Create({ specification.Name, 1280, 720 }));
        this->m_Windnow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
        this->m_Windnow->SetVSync(true);

        Hazel::Renderer::Init();

        this->m_ImGuiLayer = new ImGuiLayer();
        PushOverLayer(this->m_ImGuiLayer);

    }

    Application::~Application()
    {
        HZ_PROFILE_FUNCTION();

        if (this->m_ImGuiLayer)
            delete this->m_ImGuiLayer;
    }
    
    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
        //HZ_CORE_TRACE("{0}", e);

        for (auto it = this->m_LayerStack.end(); it != this->m_LayerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if (e.m_Handled)
                break;
        }
    }
    
    void Application::Run()
    {
        HZ_PROFILE_FUNCTION();

        while (this->is_Running)
        {
            HZ_PROFILE_SCOPE("Running Loop");

            float curtime = (float)glfwGetTime();
            TimeStep ts(curtime - this->m_LastTime);
            this->m_LastTime = curtime;

            if (!this->is_Minimum)
            {
                HZ_PROFILE_SCOPE("LayerStack OnUpdate");

                for (Layer* layer : this->m_LayerStack)
                    layer->OnUpdate(ts);

                this->m_ImGuiLayer->Begin();
                {
                    HZ_PROFILE_SCOPE("LayerStack OnImGuiRender");

                    for (Layer* layer : this->m_LayerStack)
                        layer->OnImGuiRender();
                }
                this->m_ImGuiLayer->End();
            }

            this->m_Windnow->OnUpdate();
        }
    }

    void Application::Close()
    {
        this->is_Running = false;
    }


    void Application::PushLayer(Layer* layer)
    {
        HZ_PROFILE_FUNCTION();

        this->m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverLayer(Layer* layer)
    {
        HZ_PROFILE_FUNCTION();

        this->m_LayerStack.PushOverLayer(layer);
        layer->OnAttach();
    }
    
    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        this->is_Running = false;

        return false;
    }
    
    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        HZ_PROFILE_FUNCTION();

        if (e.GetHeight() == 0 || e.GetWidth() == 0)
        {
            this->is_Minimum = true;
            return false;
        }
        this->is_Minimum = false;
        Renderer::OnResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}
