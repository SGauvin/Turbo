#include <glad/glad.h>

#include "Turbo/Core/Renderer/Abstraction/RenderCommand.h"
#include "Turbo/Core/Log.h"

namespace
{
    void openGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
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
} // namepsace

namespace Turbo
{
    namespace RenderCommand
    {
        template<>
        void init<RenderingApi::OpenGL>()
        {
#if !defined(NDEBUG)
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openGLMessageCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif // NDEBUG
        }

        template<>
        void setViewport<RenderingApi::OpenGL>(const glm::uvec2& position, const glm::uvec2& size)
        {
            glViewport(position.x, position.y, size.x, size.y);
        }

        template<>
        void setClearColor<RenderingApi::OpenGL>(const glm::vec4& color)
        {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        template<>
        void clear<RenderingApi::OpenGL>()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        template<>
        void draw<RenderingApi::OpenGL>(VertexArray<RenderingApi::OpenGL>* vertexArray)
        {
            glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
        }
    } // namspace RenderCommand
} // namespace Turbo