#pragma once

#include "Turbo/Core/Application/CompileTimeSettings.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<RenderingApi>
    class VertexArrayTemplate
    {
    public:
        VertexArrayTemplate();
        ~VertexArrayTemplate();

        void bind() const;
    };

    using VertexArray = VertexArrayTemplate<renderingApi>;
} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLVertexArray.h"
