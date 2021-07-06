#ifndef INCLUDED_TURBO_OPENGLVERTEXARRAY_H
#define INCLUDED_TURBO_OPENGLVERTEXARRAY_H

#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Renderer/Abstraction/VertexArray.h"

namespace Turbo
{
    template<>
    class VertexArray<RenderingApi::OpenGL>
    {
    public:
        VertexArray();
        ~VertexArray();

        void bind() const;

    private:
        std::uint32_t m_vertexArray;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_OPENGLVERTEXARRAY_H
