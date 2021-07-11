#include <cstdint>
#include <glm/glm.hpp>

#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Renderer/Abstraction/VertexArray.h"
#include "Turbo/Core/Renderer/Abstraction/FrameBuffer.h"

namespace Turbo
{
    namespace RenderCommand
    {
        template<RenderingApi>
        void init();

        template<RenderingApi>
        void setViewport(const glm::uvec2& position, const glm::uvec2& size);

        template<RenderingApi>
        void setClearColor(const glm::vec4& color);

        template<RenderingApi>
        void clear();

        template<RenderingApi>
        void draw(VertexArray* vertexArray);

        template<RenderingApi api>
        void beginViewport(FrameBuffer<api>* frameBuffer);

        template<RenderingApi api>
        void endViewport(FrameBuffer<api>* frameBuffer, glm::uvec2 windowSize);
    } // namspace RenderCommand
} // namespace Turbo
