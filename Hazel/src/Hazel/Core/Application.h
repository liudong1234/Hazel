#pragma once
<<<<<<< HEAD
#include "Core.h"
=======
#include "Hazel/Core/Core.h"
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
#include "Window.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Imgui/ImGuiLayer.h"
<<<<<<< HEAD
#include "Hazel/Debug/Instrumentor.h"

namespace Hazel
{
    class HAZEL_API Application
    {
    public:
        Application(const std::string& name = "Hazel Engine");
        virtual ~Application();

        void OnEvent(Event& e);
        void Run();
        void Close();

        void PushLayer(Layer* layer);
        void PushOverLayer(Layer* layer);

        ImGuiLayer* GetImGuiLayer() { return this->m_ImGuiLayer; }
        inline static Application& Get() { return *s_Instance; }
        inline Window& GetWindow() { return *this->m_Windnow; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        Scope<Window> m_Windnow;
        //std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
        ImGuiLayer* m_ImGuiLayer;
        bool is_Running = true;
        bool is_Minimum;

        LayerStack m_LayerStack;
        static Application* s_Instance;

        float m_LastTime;


    };

    Application* CreateApplication();
=======

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
		bool OnWindowResize(WindowResizeEvent& e);
		std::unique_ptr<Window> m_Windnow;
		//std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
		ImGuiLayer* m_ImGuiLayer;
		bool is_Running = true;
		bool is_Minimum;
		LayerStack m_LayerStack;
		static Application* s_Instance;

		float m_LastTime;
	};

	Application* CreateApplication();
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}

