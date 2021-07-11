#include "Turbo/Core/Renderer/OpenGL/OpenGLIndexBuffer.h"
#include <glad/glad.h>

namespace Turbo
{
    IndexBufferTemplate<RenderingApi::OpenGL>::IndexBufferTemplate(std::span<std::uint32_t> indices)
    {
        glCreateBuffers(1, &m_rendererId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(std::uint32_t), indices.data(), GL_STATIC_DRAW);
        m_count = static_cast<std::uint32_t>(indices.size());
    }

    IndexBufferTemplate<RenderingApi::OpenGL>::~IndexBufferTemplate() { glDeleteBuffers(1, &m_rendererId); }

    void IndexBufferTemplate<RenderingApi::OpenGL>::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId); }

    std::uint32_t IndexBufferTemplate<RenderingApi::OpenGL>::getCount() const { return m_count; }
} // namespace Turbo
