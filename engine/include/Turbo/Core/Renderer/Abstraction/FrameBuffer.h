#pragma once

#include "Turbo/Core/Application/CompileTimeSettings.h"
#include "Turbo/Core/Renderer/RenderingApi.h"
#include <glm/glm.hpp>

namespace Turbo
{
    template<RenderingApi>
    class FrameBufferTemplate
    {
    public:
        FrameBufferTemplate(glm::ivec2 size);
        void bind() const;
        void unbind() const;
        void resize(glm::ivec2 size);
        glm::ivec2 getSize() const;
        std::uint32_t getTexture() const;
    };

    using FrameBuffer = FrameBufferTemplate<renderingApi>;
}

#include "Turbo/Core/Renderer/OpenGL/OpenGLFrameBuffer.h"
