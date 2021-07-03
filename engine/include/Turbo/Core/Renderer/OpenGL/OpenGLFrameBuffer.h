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
        void create();
        void bind() const;
        void unbind() const;
        std::uint32_t getTexture() const;

    private:
        std::uint32_t m_frameBuffer = 0;
        std::uint32_t m_texture = 0;
        glm::ivec2 m_size;
    };
}

#endif // INCLUDED_TURBO_FRAMEBUFFER_H
