#include "Turbo/Core/Application.h"
#include "Turbo/Core/Log.h"
#include "Turbo/Core/State/State.h"

namespace Turbo
{
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
        while (m_window.isOpen() && !m_states.empty())
        {
            // Restart clock and update lag times
            auto timePointBeforeUpdate = std::chrono::high_resolution_clock::now();

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

            // Restart clock and update lag times
            auto timePointBetweenUpdateAndDraw = std::chrono::high_resolution_clock::now();
            auto elapsedTime = timePointBetweenUpdateAndDraw - timePointBeforeUpdate;
            m_updateLag += elapsedTime;
            m_drawLag += elapsedTime;

            // Draw
            if (m_drawLag >= m_timePerDraw)
            {
                float lag = static_cast<float>(m_updateLag / m_timePerUpdate);
                m_states.back()->draw(lag);
                for (const auto& layer : m_states.back()->m_layers)
                {
                    if (layer->enableDraw)
                    {
                        layer->draw(lag);
                    }
                }

                while (m_drawLag >= m_timePerDraw)
                {
                    m_drawLag -= m_timePerDraw;
                }
            }
        }
        m_window.destroy();
    }
} // namespace Turbo
