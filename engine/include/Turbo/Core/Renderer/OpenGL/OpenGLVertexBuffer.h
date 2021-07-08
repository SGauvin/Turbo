#ifndef INCLUDED_TURBO_OPENGLVERTEXBUFFER_H
#define INCLUDED_TURBO_OPENGLVERTEXBUFFER_H

#include <span>
#include <glad/glad.h>
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Renderer/BufferLayout.h"

namespace Turbo
{
    template<>
    class VertexBuffer<RenderingApi::OpenGL>
    {
    public:
        VertexBuffer(std::span<float> vertices);
        ~VertexBuffer();
        void bind() const;
        void setLayout(const BufferLayout& layout);
        const BufferLayout& getLayout() const;

    private:
        std::uint32_t m_rendererId;
        BufferLayout m_layout;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_OPENGLVERTEXBUFFER_H
