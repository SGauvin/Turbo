#ifndef INCLUDED_TURBO_OPENGLINDEXBUFFER_H
#define INCLUDED_TURBO_OPENGLINDEXBUFFER_H

#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include <span>

namespace Turbo
{
    template<>
    class IndexBuffer<RenderingApi::OpenGL>
    {
    public:
        IndexBuffer(std::span<std::uint32_t> indices);
        ~IndexBuffer();

        void bind() const;
        void unbind() const;

        std::uint32_t getCount() const;

    private:
        std::uint32_t m_rendererId;
        std::uint32_t m_count;
    };
}

#endif // INCLUDED_TURBO_OPENGLINDEXBUFFER_H
