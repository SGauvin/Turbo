#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include "Turbo/Core/Log.h"
#include "Turbo/Core/Renderer/Abstraction/RenderCommand.h"

namespace
{
    void openGLMessageCallback(unsigned /*source*/, unsigned /*type*/, unsigned /*id*/, unsigned severity, int /*length*/, const char* message,
                               const void* /*userParam*/)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            TURBO_ENGINE_ERROR(message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            TURBO_ENGINE_ERROR(message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            TURBO_ENGINE_WARNING(message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            TURBO_ENGINE_INFO(message);
            break;
        }
    }
} // namespace

namespace Turbo::RenderCommand
{
    template<>
    void initImpl<RenderingApi::OpenGL>()
    {
#if !defined(NDEBUG)
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openGLMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif // NDEBUG
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    template<>
    void setViewportImpl<RenderingApi::OpenGL>(const glm::uvec2& position, const glm::uvec2& size)
    {
        glViewport(static_cast<GLint>(position.x), static_cast<GLint>(position.y), static_cast<GLint>(size.x), static_cast<GLint>(size.y));
    }

    template<>
    void setClearColorImpl<RenderingApi::OpenGL>(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    template<>
    void clearImpl<RenderingApi::OpenGL>()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    template<>
    void drawImpl<RenderingApi::OpenGL>(VertexArray const* vertexArray)
    {
        vertexArray->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
    }
} // namespace Turbo::RenderCommand
