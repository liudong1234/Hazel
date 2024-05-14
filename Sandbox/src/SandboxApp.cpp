#include "Hazel.h"
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

private:

};

Hazel::Application* Hazel::CreateApplication()
{
    return new Sandbox();
}