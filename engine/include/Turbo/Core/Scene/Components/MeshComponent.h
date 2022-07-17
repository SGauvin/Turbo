#pragma once

#include "Turbo/Core/Application/CompileTimeSettings.h"
#include "Turbo/Core/Log.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/Texture.h"
#include "Turbo/Core/Renderer/Abstraction/VertexArray.h"
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"

namespace Turbo
{
    class MeshComponent
    {
    public:
        std::unique_ptr<VertexArray> m_vertexArray;
        std::unique_ptr<Texture> m_texture;
    };
} // namespace Turbo
