#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include "Turbo/Core/Renderer/Renderer.h"

#include "Turbo/Core/Renderer/OpenGL/OpenGLVertexBuffer.h"

namespace Turbo
{
    std::unique_ptr<VertexBuffer> VertexBuffer::create(const std::span<float> vertices)
    {
        switch (Renderer::getApi())
        {
            case RenderingApi::OpenGL:
                return std::make_unique<OpenGLVertexBuffer>(vertices);
            default:
                return nullptr;
        }
    }
} // namespace Turbo