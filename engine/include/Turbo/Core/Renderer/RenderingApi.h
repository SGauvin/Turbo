#ifndef INCLUDED_TURBO_RENDERINGAPI_H
#define INCLUDED_TURBO_RENDERINGAPI_H

namespace Turbo
{
    enum class RenderingApi
    {
        OpenGL,
        Vulkan,
        DirectX11,
        DirectX12,
        Metal,
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_RENDERINGAPI_H