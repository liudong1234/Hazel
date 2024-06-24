#include "hzpch.h"
#include "OpenGLContext.h"

<<<<<<< HEAD
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
        HZ_PROFILE_FUNCTION();

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
        HZ_PROFILE_FUNCTION();

        glfwSwapBuffers(this->m_WindowHandle);
    }
}
=======
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hazel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :
		m_WindowHandle(windowHandle)
	{
		HZ_PROFILE_FUNCTION();
		HZ_CORE_ASSERT(windowHandle, "windowhandle is null");
	}

	void OpenGLContext::Init()
	{
		HZ_PROFILE_FUNCTION();

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
		HZ_PROFILE_FUNCTION();

		glfwSwapBuffers(this->m_WindowHandle);
	}
}
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
