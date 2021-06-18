#ifndef INCLUDED_TURBO_INDEXBUFFER_H
#define INCLUDED_TURBO_INDEXBUFFER_H

#include <memory>
#include <span>
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<RenderingApi>
    class IndexBuffer
    {
    public:
        IndexBuffer(std::span<std::uint32_t> indices);
        ~IndexBuffer();
        
        void bind() const;
        void unbind() const;
        std::uint32_t getCount();
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_INDEXBUFFER_H
