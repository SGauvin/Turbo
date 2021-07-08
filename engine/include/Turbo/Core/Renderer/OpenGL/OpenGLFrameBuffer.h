#ifndef INCLUDED_TURBO_OPENGLFRAMEBUFFER_H
#define INCLUDED_TURBO_OPENGLFRAMEBUFFER_H

#include "Turbo/Core/Renderer/Abstraction/FrameBuffer.h"

namespace Turbo
{
    template<>
    class FrameBuffer<RenderingApi::OpenGL>
    {
    public:
        FrameBuffer(glm::ivec2 size);
        ~FrameBuffer();
        void bind() const;
        void unbind() const;
        void resize(glm::ivec2 size);
        glm::ivec2 getSize() const;
        std::uint32_t getTexture() const;

    private:
        void create();
        void release() const;

        std::uint32_t m_frameBuffer = 0;
        std::uint32_t m_texture = 0;
        glm::ivec2 m_size;
    };
}

#endif // INCLUDED_TURBO_FRAMEBUFFER_H
