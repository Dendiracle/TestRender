#include "Core/ApplicationWindow/WindowOpenGL.hpp"

#include "Core/Utils/Log.hpp"

#include "Core/Rendering/OpenGL/Renderer.hpp"

#include <GLFW/glfw3.h>

namespace rec
{
    WindowOpenGL::WindowOpenGL(unsigned int windowWidth, unsigned int windowHeight, const char* title) : WindowBase::WindowBase(windowWidth, windowHeight, title)
    {
        this->init();
    }

    WindowOpenGL::~WindowOpenGL()
    {
        this->shutdown();
    }

    int WindowOpenGL::init()
    {
        glfwSetErrorCallback([](int error_code, const char* description)
            {
                logCritical("GLFW error: {0}", description);
            });

        if (!glfwInit())
        {
            logCritical("Cannot initialize GLFW.");
            return -1;
        }
        logInfo("GLFW initialized.");
        

        m_pGLWindow = glfwCreateWindow(this->m_Data.width, this->m_Data.height, this->m_Data.p_Title, NULL, NULL);
        if (!m_pGLWindow)
        {
            logCritical("Cannot create GLFW window.");
            return -2;
        }
        logInfo("GLFW window is created.");

        if (!Renderer_OpenGL::init(m_pGLWindow))
        {
            logCritical("Cannot initialize OpenGL renderer.");
            return -3;
        }
        logInfo("OpenGL renderer initialized.");

        glfwSetWindowUserPointer(m_pGLWindow, &m_Data);

        glfwSetKeyCallback(m_pGLWindow,
            [](GLFWwindow* pWindow, int key, int scancode, int action, int mods)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                switch (action)
                {
                case GLFW_PRESS:
                {
                    EventKeyPressed event(static_cast<KeyCode>(key), false);
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    EventKeyReleased event(static_cast<KeyCode>(key));
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    EventKeyPressed event(static_cast<KeyCode>(key), true);
                    data.eventCallbackFn(event);
                    break;
                }
                }
            }
        );

        glfwSetMouseButtonCallback(m_pGLWindow,
            [](GLFWwindow* pWindow, int button, int action, int mods)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                double x_pos;
                double y_pos;
                glfwGetCursorPos(pWindow, &x_pos, &y_pos);
                switch (action)
                {
                case GLFW_PRESS:
                {
                    EventMouseButtonPressed event(static_cast<MouseButton>(button), x_pos, y_pos);
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    EventMouseButtonReleased event(static_cast<MouseButton>(button), x_pos, y_pos);
                    data.eventCallbackFn(event);
                    break;
                }
                }
            }
        );

        glfwSetWindowSizeCallback(m_pGLWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;
                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            }
        ); 
        
        glfwSetCursorPosCallback(m_pGLWindow,
            [](GLFWwindow* pWindow, double x, double y)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(m_pGLWindow,
            [](GLFWwindow* pWindow)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventWindowClose event;
                data.eventCallbackFn(event);
            }
        );

        glfwSetFramebufferSizeCallback(m_pGLWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                Renderer_OpenGL::set_viewport(width, height);
            }
        );

        return 0;
	}

    void WindowOpenGL::shutdown()
    {
        glfwDestroyWindow(m_pGLWindow);
        glfwTerminate();
    }

    void WindowOpenGL::onUpdate()
    {
        if (!glfwWindowShouldClose(m_pGLWindow))
        {
            glfwSwapBuffers(m_pGLWindow);
            glfwPollEvents();
        }
    }
}