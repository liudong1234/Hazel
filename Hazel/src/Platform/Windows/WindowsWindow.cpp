#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Core/Log.h"
#include "Hazel/Core/Core.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Hazel
{
    static bool s_GLFWInitialized = false;
    static uint8_t s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char* description)
    {
        HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Scope<Window> Window::Create(const WindowProps& props)
    {
        return CreateScope<WindowsWindow>(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        HZ_PROFILE_FUNCTION();
        
        Init(props);
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        HZ_PROFILE_FUNCTION();

        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        HZ_CORE_INFO("create window {0} ({1}, {2})", props.Title, props.Width, props.Height);
        if (!s_GLFWInitialized)
        {
            HZ_PROFILE_SCOPE("GlfwCreateWindow");
            int success = glfwInit();
            HZ_CORE_ASSERT(success, "Could not initialize glfw!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        {
            HZ_PROFILE_SCOPE("GlfwCreateWindow");
			#if defined(HZ_DEBUG)
				if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
					glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			#endif
            m_Window = glfwCreateWindow(int(props.Width), int(props.Height), m_Data.Title.c_str(), nullptr, nullptr);
        }

        this->m_Context = GraphicsContext::Create(m_Window);
        this->m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        this->SetVSync(true);

        //set glfw callback
        glfwSetWindowSizeCallback(this->m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;
                WindowResizeEvent event(width, height);
                data.EventCallback(event);
            });

        glfwSetWindowCloseCallback(this->m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                WindowCloseEvent event;
                data.EventCallback(event);
            });

        glfwSetKeyCallback(this->m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                    case GLFW_RELEASE:
                    {
                        KeyPressEvent event(key, 0);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_PRESS:
                    {
                        KeyReleaseEvent event(key);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_REPEAT:
                    {
                        KeyPressEvent event(key, 1);
                        data.EventCallback(event);
                        break;
                    }
                }
            });

        glfwSetCharCallback(this->m_Window, [](GLFWwindow* window, unsigned int keycode)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                KeyTypeEvent event(keycode);
                data.EventCallback(event);
            });

        glfwSetMouseButtonCallback(this->m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                    case GLFW_RELEASE:
                    {
                        MouseButtonReleaseEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_PRESS:
                    {
                        MouseButtonPressEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                }
            });

        glfwSetScrollCallback(this->m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(this->m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseMovedEvent event((float)xPos, (float)yPos);
                data.EventCallback(event);
            });
    }

    void WindowsWindow::Shutdown()
    {
        HZ_PROFILE_FUNCTION();

        glfwDestroyWindow(this->m_Window);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        HZ_PROFILE_FUNCTION();

        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        this->m_Data.VSync = enabled;
    }

    void WindowsWindow::OnUpdate()
    {
        HZ_PROFILE_FUNCTION();

        glfwPollEvents();
        this->m_Context->SwapBuffers();
    }

    WindowsWindow::~WindowsWindow()
    {
        HZ_PROFILE_FUNCTION();

        this->Shutdown();
    }

    bool WindowsWindow::IsVSync() const
    {
        return false;
    }
}
