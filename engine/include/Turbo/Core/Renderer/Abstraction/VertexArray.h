#ifndef INCLUDED_TURBO_VERTEXARRAY_H
#define INCLUDED_TURBO_VERTEXARRAY_H

#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include "Turbo/Core/Settings.h"

namespace Turbo
{
    template<RenderingApi>
    class VertexArrayTemplate
    {
    public:
        VertexArrayTemplate();
        ~VertexArrayTemplate();

        void bind() const;
    };

    using VertexArray = VertexArrayTemplate<renderingApi>;
} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLVertexArray.h"

#endif // INCLUDED_TURBO_VERTEXARRAY_H
