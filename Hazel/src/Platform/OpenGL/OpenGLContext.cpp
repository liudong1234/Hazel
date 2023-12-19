#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :
		m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "windowhandle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(this->m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initailize to glad!");

		HZ_CORE_INFO("openGL info: ");

		HZ_CORE_INFO(" vendor: {0}", (char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO(" Renderer: {0}", (char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO(" version: {0}", (char*)glGetString(GL_VERSION));

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(this->m_WindowHandle);
	}
}