#ifndef INCLUDED_TURBO_OPENGLINDEXBUFFER_H
#define INCLUDED_TURBO_OPENGLINDEXBUFFER_H

#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include <span>

namespace Turbo
{
    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(std::span<std::uint32_t> indices);
        virtual ~OpenGLIndexBuffer();

        void bind() const override;
        void unbind() const override;

        std::uint32_t getCount() const;

    private:
        std::uint32_t m_rendererId;
        std::uint32_t m_count;
    };
}

#endif // INCLUDED_TURBO_OPENGLINDEXBUFFER_H
