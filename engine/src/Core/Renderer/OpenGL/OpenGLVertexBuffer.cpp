#include "Turbo/Core/Renderer/OpenGL/OpenGLVertexBuffer.h"
#include <glad/glad.h>

namespace Turbo
{
    VertexBufferTemplate<RenderingApi::OpenGL>::VertexBufferTemplate(std::span<float> vertices)
    {
        glCreateBuffers(1, &m_rendererId);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    }

    VertexBufferTemplate<RenderingApi::OpenGL>::~VertexBufferTemplate() { glDeleteBuffers(1, &m_rendererId); }

    void VertexBufferTemplate<RenderingApi::OpenGL>::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_rendererId); }

    void VertexBufferTemplate<RenderingApi::OpenGL>::setLayout(const BufferLayout& layout)
    {
        m_layout = layout;
    }

    const BufferLayout& VertexBufferTemplate<RenderingApi::OpenGL>::getLayout() const
    {
        return m_layout;
    }
} // namespace Turbo
