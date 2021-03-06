#pragma once

#include <memory>
#include <span>
#include "Turbo/Core/Application/CompileTimeSettings.h"
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<RenderingApi>
    class IndexBufferTemplate
    {
    public:
        IndexBufferTemplate(std::span<const std::uint32_t> indices);
        ~IndexBufferTemplate();

        void bind() const;
        std::uint32_t getCount();
    };

    using IndexBuffer = IndexBufferTemplate<renderingApi>;
} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLIndexBuffer.h"
