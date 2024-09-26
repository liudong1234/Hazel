#pragma once
#include "Core.h"
#include "Window.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Imgui/ImGuiLayer.h"
#include "Hazel/Debug/Instrumentor.h"

namespace Hazel
{
    struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			HZ_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	struct ApplcationSpecification
	{
		std::string Name = "Hazel App";
		std::string WorkingDir;
		ApplicationCommandLineArgs CommandLineArgs;
	};


    class HAZEL_API Application
    {
    public:
        Application(const ApplcationSpecification& spec);
        virtual ~Application();

        void OnEvent(Event& e);
        void Run();
        void Close();

        void PushLayer(Layer* layer);
        void PushOverLayer(Layer* layer);
		ApplcationSpecification GetSpecification() const { return this->m_Specification; }

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
		ApplcationSpecification m_Specification;
    };

    // To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);

}

