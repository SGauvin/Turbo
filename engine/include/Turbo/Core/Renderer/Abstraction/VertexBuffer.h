#ifndef INCLUDED_TURBO_VERTEXBUFFER_H
#define INCLUDED_TURBO_VERTEXBUFFER_H

#include <memory>
#include <span>
#include <glad/glad.h>
#include "Turbo/Core/Log.h"
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<RenderingApi>
    class VertexBuffer
    {
    public:
        VertexBuffer(std::span<float> vertices);
        ~VertexBuffer();

        void bind() const;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_VERTEXBUFFER_H
