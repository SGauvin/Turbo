#ifndef INCLUDED_TURBO_VERTEXBUFFER_H
#define INCLUDED_TURBO_VERTEXBUFFER_H

#include <span>
#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Renderer/BufferLayout.h"

namespace Turbo
{
    template<RenderingApi>
    class VertexBuffer
    {
    public:
        VertexBuffer(std::span<float> vertices, const BufferLayout& layout);
        ~VertexBuffer();

        void bind() const;
    };
} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLVertexBuffer.h"

#endif // INCLUDED_TURBO_VERTEXBUFFER_H
