#include "hzpch.h"
#include "WindowsInput.h"
#include "Hazel/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel
{
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
}
