#ifndef INCLUDED_TURBO_VERTEXARRAY_H
#define INCLUDED_TURBO_VERTEXARRAY_H

#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"

namespace Turbo
{
    template<RenderingApi>
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void 

        void bind() const;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_VERTEXARRAY_H
