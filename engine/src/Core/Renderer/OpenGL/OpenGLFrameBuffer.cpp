#include "Turbo/Core/Renderer/OpenGL/OpenGLFrameBuffer.h"
#include "Turbo/Core/Log.h"
#include <glad/glad.h>

namespace Turbo
{
    FrameBuffer<RenderingApi::OpenGL>::FrameBuffer(glm::ivec2 size)
        : m_size(size)
    {
        create();
    }

    FrameBuffer<RenderingApi::OpenGL>::~FrameBuffer()
    {
        release();
    }

    void FrameBuffer<RenderingApi::OpenGL>::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    }

    void FrameBuffer<RenderingApi::OpenGL>::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer<RenderingApi::OpenGL>::resize(glm::ivec2 size)
    {
        m_size = size;
        release();
        create();
    }

    glm::ivec2 FrameBuffer<RenderingApi::OpenGL>::getSize() const
    {
        return m_size;
    }

    std::uint32_t FrameBuffer<RenderingApi::OpenGL>::getTexture() const
    {
        return m_texture;
    }

    void FrameBuffer<RenderingApi::OpenGL>::create()
    {
        glGenFramebuffers(1, &m_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_size.x, m_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

        glGenTextures(1, &m_depth);
        glBindTexture(GL_TEXTURE_2D, m_depth);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_size.x, m_size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            TURBO_ENGINE_ERROR("Could not create framebuffer");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void FrameBuffer<RenderingApi::OpenGL>::release() const
    {
        glDeleteTextures(1, &m_depth);
        glDeleteTextures(1, &m_texture);
        glDeleteFramebuffers(1, &m_frameBuffer);
    }
}
