#ifndef INCLUDED_TURBO_STATE_H
#define INCLUDED_TURBO_STATE_H

#include "Turbo/Core/Application.h"
#include "Turbo/Core/Input/InputManager.h"
#include "Turbo/Core/Window.h"
#include "Turbo/Core/State/Layer.h"

namespace Turbo
{
    class State
    {
    private:
        friend class Application;

    public:
        State(Application& application);
        virtual ~State();

        void pushLayer(Layer* layer);
        void removeLayer(Layer* layer);
        void popLayer();

    protected:
        Application& m_application;
        InputManager& m_inputManager;
        Window& m_window;

    private:
        virtual void handleInput() = 0;
        virtual void update() = 0;
        virtual void draw(float lag = 1.0) = 0;
        virtual void onAttach() = 0;

        std::vector<Layer*> m_layers;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_STATE_H