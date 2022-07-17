#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include "Turbo/Core/Renderer/Abstraction/FrameBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/VertexArray.h"
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    namespace RenderCommand
    {
        template<RenderingApi>
        void initImpl();

        template<RenderingApi>
        void setViewportImpl(const glm::uvec2& position, const glm::uvec2& size);

        template<RenderingApi>
        void setClearColorImpl(const glm::vec4& color);

        template<RenderingApi>
        void clearImpl();

        template<RenderingApi>
        void drawImpl(VertexArray const* vertexArray);
    } // namespace RenderCommand

} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLRenderCommand.h"

namespace Turbo
{

    namespace RenderCommand
    {
        static constexpr auto init = initImpl<renderingApi>;
        static constexpr auto setViewport = setViewportImpl<renderingApi>;
        static constexpr auto setClearColor = setClearColorImpl<renderingApi>;
        static constexpr auto clear = clearImpl<renderingApi>;
        static constexpr auto draw = drawImpl<renderingApi>;
    } // namespace RenderCommand
} // namespace Turbo
