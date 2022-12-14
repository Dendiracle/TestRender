#include "Core/Rendering/OpenGL/Renderer.hpp"

#include "Core/Utils/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Rendering/OpenGL/VertexArray.hpp"


namespace rec 
{
    bool Renderer_OpenGL::init(GLFWwindow* pWindow)
    {
        glfwMakeContextCurrent(pWindow);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            logCritical("Failed to initialize GLAD");
            return false;
        }

        logInfo("OpenGL context initialized:");
        logInfo("  Vendor: {0}", get_vendor_str());
        logInfo("  Renderer: {0}", get_renderer_str());
        logInfo("  Version: {0}", get_version_str());

        return true;
    }

    void Renderer_OpenGL::draw(const VertexArray& vertex_array)
    {
        vertex_array.bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.get_indices_count()), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer_OpenGL::set_clear_color(const float r, const float g, const float b, const float a)
    {
        glClearColor(r, g, b, a);
    }

    void Renderer_OpenGL::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer_OpenGL::set_viewport(const unsigned int width, const unsigned int height, const unsigned int left_offset, const unsigned int bottom_offset)
    {
        glViewport(left_offset, bottom_offset, width, height);
    }

    void Renderer_OpenGL::enable_depth_test()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer_OpenGL::disable_depth_test()
    {
        glDisable(GL_DEPTH_TEST);
    }

    const char* Renderer_OpenGL::get_vendor_str()
    {
        return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    }

    const char* Renderer_OpenGL::get_renderer_str()
    {
        return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    }

    const char* Renderer_OpenGL::get_version_str()
    {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }
}