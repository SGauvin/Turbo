#pragma once

#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    enum class ApplicationMode
    {
        Editor,
        Runtime
    };
    static constexpr ApplicationMode applicationMode = ApplicationMode::Editor;
    static constexpr RenderingApi renderingApi = RenderingApi::OpenGL;
} // namespace Turbo
