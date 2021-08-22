#pragma once

#include "Turbo/Core/Application.h"

namespace Turbo
{
    class Layer
    {
    public:
        Layer(Application& application);
        virtual ~Layer() {}

        virtual void onAttach() = 0;
        virtual void onDetach() = 0;

        virtual void handleInput() = 0;
        virtual void update() = 0;
        virtual void draw(float lag = 1.0) = 0;

        bool enableInputHandling = true;
        bool enableUpdate = true;
        bool enableDraw = true;

    protected:
        Application& m_application;
        InputManager& m_inputManager;
        Window<renderingApi>& m_window;
    };
} // namespace Turbo
