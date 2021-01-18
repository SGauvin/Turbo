#ifndef INCLUDED_TURBO_INDEXBUFFER_H
#define INCLUDED_TURBO_INDEXBUFFER_H

#include <memory>
#include <span>
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual std::uint32_t getCount() const = 0;

        static std::unique_ptr<IndexBuffer> create(const std::span<std::uint32_t> indices);
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_INDEXBUFFER_H
