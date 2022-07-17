#pragma once

#include <glm/glm.hpp>
#include "Turbo/Core/Application/CompileTimeSettings.h"
#include "Turbo/Core/Log.h"

namespace Turbo
{
    class Application;

    template<ApplicationMode Mode>
    class ApplicationModeHandlerImpl
    {
        ApplicationModeHandlerImpl(Application&) { TURBO_ASSERT(true, "Not implemented"); }

        void begin();
        void end();

        void beginViewport();
        void endViewport();

        glm::ivec2 getViewportSize() const;
    };

    using ApplicationModeHandler = ApplicationModeHandlerImpl<applicationMode>;
}; // namespace Turbo

#include "Turbo/Core/Application/EditorApplicationModeHandler.h"
