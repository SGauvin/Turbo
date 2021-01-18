#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Renderer.h"
#include "Turbo/Core/Renderer/OpenGL/OpenGLIndexBuffer.h"

namespace Turbo
{
    std::unique_ptr<IndexBuffer> IndexBuffer::create(const std::span<std::uint32_t> indices)
    {
        switch (Renderer::getApi())
        {
            case RenderingApi::OpenGL:
                return std::make_unique<OpenGLIndexBuffer>(indices);
            default:
                return nullptr;
        }
    }
} // namespace Turbo
