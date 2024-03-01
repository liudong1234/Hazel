#include <string>
#include "Hazel.h"
#include "Hazel/Core/EntryPoint.h"
#include "Hazel/Core/TimeStep.h"

#include "Sandbox2D.h"
#include "Sandbox3D.h"

class Sandbox :
	public Hazel::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		//PushLayer(new Sandbox3D());//有问题；；
		PushLayer(new Sandbox2D());
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
