#include "Turbo/Core/Application.h"
#include <cmath>
#include <glad/glad.h>
#include "Turbo/Core/Log.h"
#include "Turbo/Core/States/State.h"

namespace Turbo
{
    struct Clock
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> timePoint = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> restart()
        {
            auto nowTime = std::chrono::high_resolution_clock::now();
            auto elapsed = nowTime - timePoint;
            timePoint = nowTime;
            return elapsed;
        }
    };

    Application::Application(Window& window, InputManager& inputManager)
        : m_window(window)
        , m_inputManager(inputManager)
    {
    }

    Application::~Application()
    {
        for (const auto& state : m_states)
        {
            delete state;
        }
    }

    void Application::push(State* state)
    {
        m_states.push_back(state);
        m_inputManager.onStateChange(state);
        state->onAttach();
    }

    void Application::pop()
    {
        if (m_states.empty())
        {
            return;
        }

        delete m_states.back();
        m_states.pop_back();

        if (m_states.empty())
        {
            m_inputManager.onStateChange(nullptr);
        }
        else
        {
            m_inputManager.onStateChange(m_states.back());
        }
    }

    void Application::start()
    {
        Clock clock;
        while (m_window.isOpen() && !m_states.empty())
        {
            {
                auto elapsed = clock.restart();
                m_updateLag += elapsed;
                m_drawLag += elapsed;
            }

            // HandleInput and Update on a fixed timestep (skip draw until caught up)
            while (m_updateLag >= m_timePerUpdate)
            {
                // InputManager update
                m_window.processEvents();

                // Handle input
                m_states.back()->handleInput();
                for (std::int32_t i = static_cast<std::int32_t>(m_states.back()->m_layers.size()) - 1; i >= 0; --i)
                {
                    if (m_states.back()->m_layers[i]->enableInputHandling)
                    {
                        m_states.back()->m_layers[i]->handleInput();
                    }
                }

                // Update
                m_states.back()->update();
                for (std::int32_t i = static_cast<std::int32_t>(m_states.back()->m_layers.size()) - 1; i >= 0; --i)
                {
                    if (m_states.back()->m_layers[i]->enableUpdate)
                    {
                        m_states.back()->m_layers[i]->update();
                    }
                }

                m_updateLag -= m_timePerUpdate;
            }

            if (m_states.empty())
            {
                m_drawLag = std::chrono::nanoseconds(0);
                m_updateLag = std::chrono::nanoseconds(0);
                continue;
            }

            {
                auto elapsed = clock.restart();
                m_updateLag += elapsed;
                m_drawLag += elapsed;
            }

            // Draw
            if (m_drawLag >= m_timePerDraw)
            {
                m_window.clear();
                float lag = static_cast<float>(m_updateLag / m_timePerUpdate);
                m_states.back()->draw(lag);
                for (const auto& layer : m_states.back()->m_layers)
                {
                    if (layer->enableDraw)
                    {
                        layer->draw(lag);
                    }
                }

                m_window.swapBuffers();

                m_drawLag -= std::floor(m_drawLag / m_timePerDraw) * m_timePerDraw;
            }
        }
        m_window.destroy();
    }
} // namespace Turbo
