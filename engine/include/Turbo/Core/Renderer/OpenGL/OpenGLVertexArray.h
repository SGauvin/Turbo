#ifndef INCLUDED_TURBO_OPENGLVERTEXARRAY_H
#define INCLUDED_TURBO_OPENGLVERTEXARRAY_H

#include <memory>

#include "Turbo/Core/Renderer/Abstraction/VertexArray.h"
#include "Turbo/Core/Renderer/OpenGL/OpenGLVertexBuffer.h"
#include "Turbo/Core/Renderer/OpenGL/OpenGLIndexBuffer.h"
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<>
    class VertexArray<RenderingApi::OpenGL>
    {
    public:
        VertexArray();
        ~VertexArray();

        void bind() const;

        void setVertexBuffer(std::shared_ptr<VertexBuffer<RenderingApi::OpenGL>> vertexBuffer);
        void setIndexBuffer(std::shared_ptr<IndexBuffer<RenderingApi::OpenGL>> indexBuffer);

        IndexBuffer<RenderingApi::OpenGL>* getIndexBuffer() const;

    private:
        std::uint32_t m_vertexArray;
        std::shared_ptr<VertexBuffer<RenderingApi::OpenGL>> m_vertexBuffer;
        std::shared_ptr<IndexBuffer<RenderingApi::OpenGL>> m_indexBuffer;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_OPENGLVERTEXARRAY_H
