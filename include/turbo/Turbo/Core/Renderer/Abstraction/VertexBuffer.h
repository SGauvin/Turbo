#ifndef INCLUDED_TURBO_VERTEXBUFFER_H
#define INCLUDED_TURBO_VERTEXBUFFER_H

#include <memory>
#include <span>

namespace Turbo
{
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        static std::unique_ptr<VertexBuffer> create(const std::span<float> vertices);
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_VERTEXBUFFER_H
