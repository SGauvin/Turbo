#pragma once

#include "Turbo/Core/Application/ApplicationModeHandler.h"
#include "Turbo/Core/Renderer/Abstraction/FrameBuffer.h"

namespace Turbo
{
    template<>
    class ApplicationModeHandlerImpl<ApplicationMode::Editor>
    {
    public:
        ApplicationModeHandlerImpl(Application& application);

        void begin();
        void end();

        void beginViewport();
        void endViewport();

        glm::ivec2 getViewportSize() const;

    private:
        Application& m_application;
        FrameBuffer m_frameBuffer;
        glm::uvec2 m_oldViewportSize = {0, 0};
    };
}
