#pragma once

#include <glm/glm.hpp>
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<RenderingApi>
    class Shader
    {
    };

} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLShader.h"
