#ifndef INCLUDED_TURBO_INDEXBUFFER_H
#define INCLUDED_TURBO_INDEXBUFFER_H

#include <memory>
#include <span>
#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Settings.h"

namespace Turbo
{
    template<RenderingApi>
    class IndexBufferTemplate
    {
    public:
        IndexBufferTemplate(std::span<std::uint32_t> indices);
        ~IndexBufferTemplate();

        void bind() const;
        std::uint32_t getCount();
    };

    using IndexBuffer = IndexBufferTemplate<renderingApi>;
} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLIndexBuffer.h"

#endif // INCLUDED_TURBO_INDEXBUFFER_H
