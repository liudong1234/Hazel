#include "hzpch.h"
<<<<<<< HEAD
#include "Hazel/Core/Input.h"
=======
#include "WindowsInput.h"
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
#include "Hazel/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel
{
<<<<<<< HEAD
    //Input* Input::s_Instance = new Input();

    bool Input::IsKeyPressed(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        auto state = glfwGetKey(window, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
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
=======
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, keycode);

		return state == GLFW_PRESS;
	}
	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);

		return { (float)x, (float)y };
	}
	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = this->GetMousePositionImpl();
		return (float)x;
	}
	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = this->GetMousePositionImpl();
		return (float)y;
	}
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}
