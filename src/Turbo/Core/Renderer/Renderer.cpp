#include "Turbo/Core/Renderer/Renderer.h"

namespace Turbo
{
    namespace
    {
        RenderingApi renderingApi = RenderingApi::OpenGL;
    } // namespace

    namespace Renderer
    {
        void setApi(RenderingApi api) { renderingApi = api; }

        RenderingApi getApi() { return renderingApi; }
    } // namespace Renderer
} // namespace Turbo