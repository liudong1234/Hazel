#include "hzpch.h"
#include "Application.h"
#include "Input.h"

#include "Hazel/Renderer/Renderer.h"

#include <glad/glad.h>
namespace Hazel
{
	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;


	Application::Application() :
		m_Camera(-10.0f, 10.0f, -10.0f, 10.0f)
	{
		HZ_CORE_ASSERT(!s_Instance, "application already exist! ");
		s_Instance = this;
		this->m_Windnow = std::unique_ptr<Window>(Window::Create());
		this->m_Windnow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		this->m_ImGuiLayer = new ImGuiLayer();
		PushOverLayer(this->m_ImGuiLayer);

		this->m_VertexArray.reset(VertexArray::Create());
		float vertices[] =
		{
			-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout =
		{
			{ "g_Position", ShaderDataType::Float3 },
			{ "g_Color", ShaderDataType::Float4 }
		};
		m_VertexBuffer->SetLayout(layout);
		this->m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		this->m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc =
			R"(
			#version 330 core
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;
			out vec4 v_Color;
			out vec3 v_Position;
			uniform mat4 projection;
			uniform mat4 view;
			void main()
			{
				v_Position = a_Position;
				gl_Position = projection * view * vec4(a_Position, 1.0f);
				v_Color = a_Color;
			}
		)";

		std::string fragmentSrc =
			R"(
			#version 330 core
			//layout(location=0) out vec3 a_Position;
			layout(location=0) out vec4 color;
			in vec4 v_Color;
			in vec3 v_Position;
			void main()
			{
				color = v_Color;
			}
		)";
		this->shader.reset(new Shader(vertexSrc, fragmentSrc));

		float quadVertices[] =
		{
			-0.5f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f,  -0.5f, 0.0f,
			0.5f,   0.5f, 0.0f
		};
		this->quadVa.reset(VertexArray::Create());
		m_VertexBuffer.reset(VertexBuffer::Create(quadVertices, sizeof(quadVertices)));

		layout = { { "g_Position", ShaderDataType::Float3 } };
		m_VertexBuffer->SetLayout(layout);
		this->quadVa->AddVertexBuffer(m_VertexBuffer);

		uint32_t quadIndices[] = { 0, 1, 2, 0, 3, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
		this->quadVa->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc2 =
			R"(
			#version 330 core
			layout(location=0) in vec3 a_Position;
			uniform mat4 projection;
			uniform mat4 view;
			void main()
			{
				gl_Position = projection * view * vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc2 =
			R"(
			#version 330 core
			layout(location=0) out vec4 color;
			void main()
			{
				color = vec4(0.1f, 0.3f, 0.2f, 1.0f);
			}
		)";
		this->quadShader.reset(new Shader(vertexSrc2, fragmentSrc2));
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

			RenderCommand::SetClearColor({ 1.0f, 0.1f, 1.0f, 1.0f });
			RenderCommand::Clear();

			this->m_Camera.SetPosition({ 1.0f, 0.0f, 0.0f });
			this->m_Camera.SetRotation(180.0f);

			Renderer::BeginScene(this->m_Camera);
			Renderer::Submit(this->quadVa, this->quadShader);
			Renderer::Submit(this->m_VertexArray, this->shader);
			Renderer::EndScend();



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
