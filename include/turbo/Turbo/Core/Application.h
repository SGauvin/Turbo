#ifndef INCLUDED_TURBO_APPLICATION_H
#define INCLUDED_TURBO_APPLICATION_H

#include <chrono>
#include <numeric>
#include <vector>
#include "Turbo/Core/Window/Window.h"
#include "Turbo/Core/Renderer/OpenGL/OpenGLShader.h"

namespace Turbo
{
    class State;

    class Application
    {
    public:
        Application(Window& window, InputManager& inputManager);
        ~Application();

        void push(State* state);
        void pop();

        void start();

        Window& getWindow() { return m_window; }
        InputManager& getInputManager() { return m_inputManager; }

    private:
        Window& m_window;
        InputManager& m_inputManager;
        OpenGLShader m_shader;

        std::vector<State*> m_states;

        std::uint32_t m_vertexArray;
        std::uint32_t m_vertexBuffer;
        std::uint32_t m_indexBuffer;

        std::chrono::duration<double> m_timePerUpdate = std::chrono::nanoseconds(1000000000 / 60);
        std::chrono::duration<double> m_timePerDraw = std::chrono::nanoseconds(1000000000 / 200000);
        std::chrono::duration<double> m_updateLag{0};
        std::chrono::duration<double> m_drawLag{0};
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_APPLICATION_H