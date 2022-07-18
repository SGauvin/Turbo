#include "Turbo/Core/Renderer/OpenGL/OpenGLIndexBuffer.h"
#include <glad/glad.h>

namespace Turbo
{

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init) -- m_rendererId is being assigned with glCreateBuffers
    IndexBufferTemplate<RenderingApi::OpenGL>::IndexBufferTemplate(std::span<const std::uint32_t> indices)
        : m_count(static_cast<std::uint32_t>(indices.size()))
    {
        glCreateBuffers(1, &m_rendererId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(std::uint32_t)), indices.data(), GL_STATIC_DRAW);
    }

    IndexBufferTemplate<RenderingApi::OpenGL>::~IndexBufferTemplate()
    {
        glDeleteBuffers(1, &m_rendererId);
    }

    void IndexBufferTemplate<RenderingApi::OpenGL>::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
    }

    std::uint32_t IndexBufferTemplate<RenderingApi::OpenGL>::getCount() const
    {
        return m_count;
    }
} // namespace Turbo
