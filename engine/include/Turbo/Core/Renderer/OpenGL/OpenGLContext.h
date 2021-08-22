#pragma once

struct GLFWwindow;

namespace Turbo
{
    class OpenGLContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        void init();
        void swapBuffers();

    private:
        GLFWwindow* m_windowHandle;
    };
} // namespace Turbo
