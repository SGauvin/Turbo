#ifndef INCLUDED_TURBO_OPENGLINDEXBUFFER_H
#define INCLUDED_TURBO_OPENGLINDEXBUFFER_H

#include <span>
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"

namespace Turbo
{
    template<>
    class IndexBufferTemplate<RenderingApi::OpenGL>
    {
    public:
        IndexBufferTemplate(std::span<std::uint32_t> indices);
        ~IndexBufferTemplate();

        void bind() const;

        std::uint32_t getCount() const;

    private:
        std::uint32_t m_rendererId;
        std::uint32_t m_count;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_OPENGLINDEXBUFFER_H
