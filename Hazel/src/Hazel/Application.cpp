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
		//this->m_ImGuiLayer = std::make_unique<ImGuiLayer>();
		this->m_ImGuiLayer = new ImGuiLayer();
		PushOverLayer(this->m_ImGuiLayer);


		glGenVertexArrays(1, &this->m_VertexArray);
		glBindVertexArray(this->m_VertexArray);


		float vertices[] =
		{
			-0.5f, 0.0f, 0.0f,
			0.5f,  0.0f, 0.0f,
			0.0f,  0.5f, 0.0f
		};
		this->m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		//this->m_VertexBuffer->Bind();
		uint32_t indices[] =
		{
			0, 1, 2
		};
		this->m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		//this->m_IndexBuffer->Bind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, this->m_IndexBuffer->GetCount(), GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);


		std::string vertexSrc =
			R"(
			#version 330 core
			layout(location=0) in vec3 a_Position;
			void main()
			{
				gl_Position = vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc =
			R"(
			#version 330 core
			//layout(location=0) out vec3 a_Position;
			out vec4 color;
			void main()
			{
				color = vec4(0.8f, 0.2f, 0.1f, 1.0f);
			}
		)";
		this->shader.reset(new Shader(vertexSrc, fragmentSrc));
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
			glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			this->shader->Bind();
			glBindVertexArray(this->m_VertexArray);
			glDrawElements(GL_TRIANGLES, this->m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : this->m_LayerStack)
				layer->OnUpdate();

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
