#include "hzpch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>
namespace Hazel
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "application already exist! ");
		s_Instance = this;
		this->m_Windnow = std::unique_ptr<Window>(Window::Create());
		this->m_Windnow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
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
			glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : this->m_LayerStack)
				layer->OnUpdate();

			HZ_CORE_TRACE("{0}, {1}", Input::GetMouseX(), Input::GetMouseY());
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
