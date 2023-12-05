#include "Hazel.h"

#include <string>
class ExampleLayer :
	public Hazel::Layer
{
public:
	ExampleLayer() :
		Layer(" ")
	{

	}

	void OnUpdate() override
	{
		HZ_INFO("EXAMPLE layer: update");
	}

	void OnEvent(Hazel::Event& e) override
	{
		HZ_TRACE("{0}", e);
	}
};

class Sandbox :
	public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverLayer(new Hazel::ImGuiLayer());
	}
	~Sandbox()
	{

	}

private:

};
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}