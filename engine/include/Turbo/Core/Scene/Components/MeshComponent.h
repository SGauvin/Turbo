#pragma once

#include <glm/glm.hpp>
#include <tiny_gltf.h>
#include <algorithm>

#include "Turbo/Core/Log.h"
#include "Turbo/Core/Settings.h"
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/VertexArray.h"
#include "Turbo/Core/Renderer/Abstraction/Texture.h"

namespace Turbo
{
    class MeshComponent
    {
    public:
        MeshComponent()
        {
        }

        std::unique_ptr<VertexArray> m_vertexArray;
        std::unique_ptr<Texture> m_texture;
    };
} // namespace Turbo
