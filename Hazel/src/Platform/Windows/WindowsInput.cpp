#include "hzpch.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel
{
    //Input* Input::s_Instance = new Input();

    bool Input::IsKeyPressed(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        auto state = glfwGetKey(window, keycode);

        return state == GLFW_PRESS;
    }
    bool Input::IsMouseButtonPressed(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, keycode);

        return state == GLFW_PRESS;
    }
    std::pair<float, float> Input::GetMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double x;
        double y;
        glfwGetCursorPos(window, &x, &y);

        return { (float)x, (float)y };
    }
    float Input::GetMouseX()
    {
        auto [x, y] = GetMousePosition();
        return (float)x;
    }
    float Input::GetMouseY()
    {
        auto [x, y] = GetMousePosition();
        return (float)y;
    }
}
