#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include "Turbo/Core/Renderer/Abstraction/FrameBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/RenderCommand.h"
#include "Turbo/Core/Renderer/Abstraction/VertexArray.h"
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    namespace RenderCommand
    {
        template<>
        void initImpl<RenderingApi::OpenGL>();

        template<>
        void setViewportImpl<RenderingApi::OpenGL>(const glm::uvec2& position, const glm::uvec2& size);

        template<>
        void setClearColorImpl<RenderingApi::OpenGL>(const glm::vec4& color);

        template<>
        void clearImpl<RenderingApi::OpenGL>();

        template<>
        void drawImpl<RenderingApi::OpenGL>(VertexArray const* vertexArray);
    } // namespace RenderCommand
} // namespace Turbo
