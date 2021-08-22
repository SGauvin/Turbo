#pragma once

#include <span>
#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Renderer/BufferLayout.h"
#include "Turbo/Core/Settings.h"

namespace Turbo
{
    template<RenderingApi>
    class VertexBufferTemplate
    {
    public:
        VertexBufferTemplate(std::span<float> vertices);
        ~VertexBufferTemplate();

        void bind() const;
        void setLayout(const BufferLayout& layout);
        const BufferLayout& getLayout() const;
    };

    using VertexBuffer = VertexBufferTemplate<renderingApi>;
} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLVertexBuffer.h"
