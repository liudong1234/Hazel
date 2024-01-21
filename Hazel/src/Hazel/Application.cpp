#include "hzpch.h"
#include "Application.h"
#include "Input.h"
#include "Hazel/Renderer/Renderer.h"
#include <GLFW/glfw3.h>

namespace Hazel
{
	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;


	Application::Application() :
		m_LastTime(0.0f)
	{
		HZ_CORE_ASSERT(!s_Instance, "application already exist! ");
		s_Instance = this;
		this->m_Windnow = std::unique_ptr<Window>(Window::Create({ "Game Engine", 720, 960 }));
		this->m_Windnow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		this->m_Windnow->SetVSync(true);

		Hazel::Renderer::Init();

		this->m_ImGuiLayer = new ImGuiLayer();
		PushOverLayer(this->m_ImGuiLayer);

	}

	Application::~Application()
	{
		if (this->m_ImGuiLayer)
			delete this->m_ImGuiLayer;
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
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
		while (this->is_Running)
		{
			float curtime = (float)glfwGetTime();
			TimeStep ts(curtime - this->m_LastTime);
			this->m_LastTime = curtime;

			for (Layer* layer : this->m_LayerStack)
				layer->OnUpdate(ts);

			this->m_ImGuiLayer->Begin();
			for (Layer* layer : this->m_LayerStack)
				layer->OnImGuiRender();
			this->m_ImGuiLayer->End();

			//HZ_CORE_TRACE("{0}, {1}", Input::GetMouseX(), Input::GetMouseY());
			this->m_Windnow->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		this->m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* layer)
	{
		this->m_LayerStack.PushOverLayer(layer);
		layer->OnAttach();
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		this->is_Running = false;

		return false;
	}
}
