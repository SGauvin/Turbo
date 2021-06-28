#ifndef INCLUDED_TURBO_STATE_H
#define INCLUDED_TURBO_STATE_H

#include "Turbo/Core/Application.h"
#include "Turbo/Core/Input/InputManager.h"
#include "Turbo/Core/Layers/Layer.h"
#include "Turbo/Core/Window/Window.h"

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
        Window<renderingApi>& m_window;
        std::vector<Layer*> m_layers;

    private:
        virtual void handleInput() = 0;
        virtual void update() = 0;
        virtual void draw(float lag = 1.0) = 0;
        virtual void onAttach() = 0;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_STATE_H