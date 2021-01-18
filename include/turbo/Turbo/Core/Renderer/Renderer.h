#ifndef INCLUDED_TURBO_RENDERER_H
#define INCLUDED_TURBO_RENDERER_H

#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    namespace Renderer
    {
        void setApi(RenderingApi renderingApi);
        RenderingApi getApi();
    } // namespace Renderer
} // namespace Turbo

#endif // INCLUDED_TURBO_RENDERER_H
