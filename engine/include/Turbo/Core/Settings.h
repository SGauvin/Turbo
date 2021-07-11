#ifndef INCLUDED_TURBO_SETTINGS_H
#define INCLUDED_TURBO_SETTINGS_H

#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    static constexpr RenderingApi renderingApi = RenderingApi::OpenGL;
    static constexpr bool isEditorEnabled = false;
} // namespace Turbo

#endif // INCLUDED_TURBO_SETTINGS_H
