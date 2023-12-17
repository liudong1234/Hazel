#include "Hazel.h"

#include <string>

#include"imgui/imgui.h"

class ExampleLayer :
	public Hazel::Layer
{
public:
	ExampleLayer() :
		Layer("Example")
	{

	}

	void OnUpdate() override
	{
		HZ_INFO("EXAMPLE layer: update");
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("tag is pressed!");
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("hello world");
		ImGui::End();
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