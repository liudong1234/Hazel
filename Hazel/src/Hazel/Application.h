#pragma once
#include "Core.h"
#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Hazel/Imgui/ImGuiLayer.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
namespace Hazel
{
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);
		void Run();

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *this->m_Windnow; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Windnow;
		//std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
		ImGuiLayer* m_ImGuiLayer;
		bool is_Running = true;

		LayerStack m_LayerStack;
		static Application* s_Instance;


		unsigned int m_VertexArray;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	};

	Application* CreateApplication();
}

