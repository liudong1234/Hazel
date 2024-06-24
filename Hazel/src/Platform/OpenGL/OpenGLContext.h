#pragma once
#include "Hazel/Renderer/GraphicsContext.h"

<<<<<<< HEAD
//#include <GLFW/glfw3.h>
=======
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
struct GLFWwindow;

namespace Hazel
{
	class OpenGLContext :
		public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}

