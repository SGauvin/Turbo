#ifndef INCLUDED_TURBO_OPENGLVERTEXBUFFER_H
#define INCLUDED_TURBO_OPENGLVERTEXBUFFER_H

#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include <span>

namespace Turbo
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(std::span<float> vertices);
        virtual ~OpenGLVertexBuffer();

        void bind() const override;
        void unbind() const override;

    private:
        std::uint32_t m_rendererId;
    };
}

#endif // INCLUDED_TURBO_OPENGLVERTEXBUFFER_H
