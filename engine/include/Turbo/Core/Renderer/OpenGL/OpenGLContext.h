#ifndef INCLUDED_TURBO_OPENGLCONTEXT_H
#define INCLUDED_TURBO_OPENGLCONTEXT_H

class GLFWwindow;

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

#endif // INCLUDED_TURBO_OPENGLCONTEXT_H