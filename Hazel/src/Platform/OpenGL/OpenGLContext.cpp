#include "hzpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/glut.h>
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

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(this->m_WindowHandle);
	}
}