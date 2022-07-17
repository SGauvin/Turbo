#pragma once

#include "Turbo/Core/Renderer/Abstraction/FrameBuffer.h"

namespace Turbo
{
    template<>
    class FrameBufferTemplate<RenderingApi::OpenGL>
    {
    public:
        FrameBufferTemplate(glm::ivec2 size, FrameBufferSettings settings = {});
        ~FrameBufferTemplate();
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
        std::uint32_t m_depth = 0;
        glm::ivec2 m_size;

        FrameBufferSettings m_settings;
    };
} // namespace Turbo
