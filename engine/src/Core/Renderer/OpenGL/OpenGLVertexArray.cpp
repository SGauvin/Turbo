#include "Turbo/Core/Renderer/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Turbo
{
    VertexArray<RenderingApi::OpenGL>::VertexArray()
    {
        glGenVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);
    }


    VertexArray<RenderingApi::OpenGL>::~VertexArray()
    {

    }

    void VertexArray<RenderingApi::OpenGL>::bind() const
    {

    }
}