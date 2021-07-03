#ifndef INCLUDED_TURBO_FRAMEBUFFER_H
#define INCLUDED_TURBO_FRAMEBUFFER_H

#include "Turbo/Core/Renderer/RenderingApi.h"
#include <glm/glm.hpp>

namespace Turbo
{
    template<RenderingApi>
    class FrameBuffer
    {
    public:
        FrameBuffer(glm::ivec2 size);
        void bind() const;
        void unbind() const;
        void resize(glm::ivec2 size);
        std::uint32_t getTexture() const;
    };
}

#include "Turbo/Core/Renderer/OpenGL/OpenGLFrameBuffer.h"

#endif // INCLUDED_TURBO_FRAMEBUFFER_H
