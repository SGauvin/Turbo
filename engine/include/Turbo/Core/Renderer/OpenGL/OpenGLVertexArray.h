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
    class VertexArrayTemplate<RenderingApi::OpenGL>
    {
    public:
        VertexArrayTemplate();
        ~VertexArrayTemplate();

        void bind() const;

        void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
        void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

        IndexBuffer* getIndexBuffer() const;

    private:
        std::uint32_t m_vertexArray;
        std::shared_ptr<VertexBuffer> m_vertexBuffer;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_OPENGLVERTEXARRAY_H
