#pragma once

#include <span>
#include <glad/glad.h>
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include "Turbo/Core/Renderer/BufferLayout.h"
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<>
    class VertexBufferTemplate<RenderingApi::OpenGL>
    {
    public:
        VertexBufferTemplate(std::span<float> vertices);
        ~VertexBufferTemplate();
        void bind() const;
        void setLayout(const BufferLayout& layout);
        const BufferLayout& getLayout() const;

    private:
        std::uint32_t m_rendererId;
        BufferLayout m_layout;
    };
} // namespace Turbo
