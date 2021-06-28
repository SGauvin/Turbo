#include "Turbo/Core/Renderer/OpenGL/OpenGLIndexBuffer.h"
#include <glad/glad.h>

namespace Turbo
{
    IndexBuffer<RenderingApi::OpenGL>::IndexBuffer(std::span<std::uint32_t> indices)
    {
        glCreateBuffers(1, &m_rendererId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(std::uint32_t), indices.data(), GL_STATIC_DRAW);
        m_count = indices.size();
    }

    IndexBuffer<RenderingApi::OpenGL>::~IndexBuffer() { glDeleteBuffers(1, &m_rendererId); }

    void IndexBuffer<RenderingApi::OpenGL>::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId); }

    void IndexBuffer<RenderingApi::OpenGL>::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    std::uint32_t IndexBuffer<RenderingApi::OpenGL>::getCount() const { return m_count; }
} // namespace Turbo
