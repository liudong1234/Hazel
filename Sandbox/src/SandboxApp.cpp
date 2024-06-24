<<<<<<< HEAD
﻿#include "Hazel.h"
#include "Hazel/Core/EntryPoint.h"

#include <string>

#include"imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Hazel/Core/TimeStep.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Sandbox2D.h"


class Sandbox :
    public Hazel::Application
{
public:
    Sandbox()
    {
        //PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }
    ~Sandbox()
    {

    }
=======
﻿#include <string>
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
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7

private:

};

<<<<<<< HEAD
Hazel::Application* Hazel::CreateApplication()
{
    return new Sandbox();
}
=======

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
