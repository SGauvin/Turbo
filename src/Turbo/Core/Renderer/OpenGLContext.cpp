#include "Turbo/Core/Renderer/OpenGLContext.h"
#include <cstdint>
#include "Turbo/Core/Log.h"

// Include glad before glfw
#include <glad/glad.h>
// ~ Glad

#include <GLFW/glfw3.h>

namespace Turbo
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_windowHandle(windowHandle)
    {
    }

    void OpenGLContext::init()
    {
        glfwMakeContextCurrent(m_windowHandle);

        std::int32_t gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (gladStatus == 0)
        {
            TURBO_ENGINE_ERROR("Error loading OpenGL functions with glad");
        }
    }

    void OpenGLContext::swapBuffers() { glfwSwapBuffers(m_windowHandle); }

} // namespace Turbo
