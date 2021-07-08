#include "Turbo/Core/Renderer/OpenGL/OpenGLVertexBuffer.h"
#include <glad/glad.h>

namespace Turbo
{
    VertexBuffer<RenderingApi::OpenGL>::VertexBuffer(std::span<float> vertices)
    {
        glCreateBuffers(1, &m_rendererId);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    }

    VertexBuffer<RenderingApi::OpenGL>::~VertexBuffer() { glDeleteBuffers(1, &m_rendererId); }

    void VertexBuffer<RenderingApi::OpenGL>::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_rendererId); }

    void VertexBuffer<RenderingApi::OpenGL>::setLayout(const BufferLayout& layout)
    {
        m_layout = layout;
    }

    const BufferLayout& VertexBuffer<RenderingApi::OpenGL>::getLayout() const
    {
        return m_layout;
    }
} // namespace Turbo
