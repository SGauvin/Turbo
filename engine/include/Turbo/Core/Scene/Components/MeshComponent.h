#ifndef INCLUDED_TURBO_MESHCOMPONENT_H
#define INCLUDED_TURBO_MESHCOMPONENT_H

#include <glm/glm.hpp>

#include "Turbo/Core/Settings.h"
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/VertexArray.h"

namespace Turbo
{
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
    };

    class MeshComponent
    {
    public:
        MeshComponent() = default;

    private:
        
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_MESHCOMPONENT_H
