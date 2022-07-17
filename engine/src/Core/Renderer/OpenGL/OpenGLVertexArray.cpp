#include "Turbo/Core/Renderer/OpenGL/OpenGLVertexArray.h"
#include <glad/glad.h>
#include "Turbo/Core/Log.h"

namespace
{
    GLenum getOpenGLType(Turbo::DataType dataType)
    {
        static constexpr std::array dataTypes = std::to_array<GLenum>({
            GL_FLOAT, // Float
            GL_FLOAT, // Float2
            GL_FLOAT, // Float3
            GL_FLOAT, // Float4
            GL_INT, // Int
            GL_INT, // Int2
            GL_INT, // Int3
            GL_INT, // Int4
            GL_BOOL, // Bool
        });
        return dataTypes[static_cast<std::uint8_t>(dataType)];
    }
} // namespace

namespace Turbo
{
    VertexArrayTemplate<RenderingApi::OpenGL>::VertexArrayTemplate()
    {
        glGenVertexArrays(1, &m_vertexArray);
    }

    VertexArrayTemplate<RenderingApi::OpenGL>::~VertexArrayTemplate()
    {
        glDeleteVertexArrays(1, &m_vertexArray);
    }

    void VertexArrayTemplate<RenderingApi::OpenGL>::bind() const
    {
        glBindVertexArray(m_vertexArray);
        if (m_vertexBuffer != nullptr)
        {
            m_vertexBuffer->bind();
        }
        if (m_indexBuffer != nullptr)
        {
            m_indexBuffer->bind();
        }
    }

    void VertexArrayTemplate<RenderingApi::OpenGL>::setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
    {
        TURBO_ASSERT(vertexBuffer->getLayout().size() > 0, "Vertex buffer has no layout");

        bind();
        vertexBuffer->bind();

        const BufferLayout& layout = vertexBuffer->getLayout();
        for (std::size_t i = 0; i < layout.size(); i++)
        {
            glEnableVertexAttribArray(static_cast<std::uint32_t>(i));
            glVertexAttribPointer(static_cast<std::uint32_t>(i),
                                  layout[i].getComponentCount(),
                                  ::getOpenGLType(layout[i].getDataType()),
                                  layout[i].isNormalized(),
                                  layout.getStride(),
                                  reinterpret_cast<void*>(layout[i].getOffset()));
        }

        m_vertexBuffer = vertexBuffer;
    }

    void VertexArrayTemplate<RenderingApi::OpenGL>::setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
    {
        bind();
        indexBuffer->bind();

        m_indexBuffer = indexBuffer;
    }

    IndexBuffer* VertexArrayTemplate<RenderingApi::OpenGL>::getIndexBuffer() const
    {
        return m_indexBuffer.get();
    }
} // namespace Turbo
