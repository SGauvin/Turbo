#include "Turbo/Core/Application/Application.h"
#include <cmath>
#include "Turbo/Core/Log.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/RenderCommand.h"
#include "Turbo/Core/Renderer/BufferLayout.h"
#include "Turbo/Core/States/State.h"
#include "Turbo/Core/Window/OpenGLWindow.h"

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

    Application::Application(const WindowAttributes& windowAttributes)
        : m_inputManager()
        , m_window((initLogs(), windowAttributes), m_inputManager)
        , m_applicationModeHandler((RenderCommand::init(), *this))
    {
    }

    Application::~Application()
    {
        for (const auto& state : m_states)
        {
            state->~State();
            delete[] reinterpret_cast<std::uint8_t*>(state);
        }
    }

    void Application::pop()
    {
        if (m_states.empty())
        {
            return;
        }

        m_inputManager.onStateRemove(m_states.back());

        delete m_states.back();
        m_states.pop_back();

        m_inputManager.onStateChange(m_states.empty() ? nullptr : m_states.back());
    }

    void Application::setTargetUps(double ups)
    {
        m_timePerUpdate = std::chrono::nanoseconds(static_cast<std::uint32_t>(std::round(1000000000.f / ups)));
    }

    void Application::setTargetFps(double fps)
    {
        m_timePerDraw = std::chrono::nanoseconds(static_cast<std::uint32_t>(std::round(1000000000.f / fps)));
    }

    void Application::start()
    {
        Clock clock;

        while (m_window.isOpen() && !m_states.empty())
            [[likely]]
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

                if (m_states.empty()) [[unlikely]]
                {
                    m_drawLag = std::chrono::nanoseconds(0);
                    m_updateLag = std::chrono::nanoseconds(0);
                    break;
                }

                {
                    auto elapsed = clock.restart();
                    m_updateLag += elapsed;
                    m_drawLag += elapsed;
                }

                if (m_drawLag >= m_timePerDraw)
                {
                    if (m_applicationModeHandler.getViewportSize().x > 0.f)
                    {
                        RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1.f});
                        RenderCommand::clear();

                        m_applicationModeHandler.begin();
                        m_applicationModeHandler.beginViewport();

                        // Calculate lag for draw interpolation
                        auto lag = static_cast<float>(m_updateLag / m_timePerUpdate);
                        m_states.back()->draw(lag);
                        for (const auto& layer : m_states.back()->m_layers)
                        {
                            if (layer->enableDraw)
                            {
                                layer->draw(lag);
                            }
                        }

                        m_applicationModeHandler.endViewport();
                        m_applicationModeHandler.end();

                        m_window.swapBuffers();
                    }

                    m_drawLag -= std::floor(m_drawLag / m_timePerDraw) * m_timePerDraw;
                }
            }
    }

    glm::uvec2 Application::getViewportSize() const
    {
        return m_applicationModeHandler.getViewportSize();
    }

    float Application::getViewportAspectRatio() const
    {
        return static_cast<float>(m_applicationModeHandler.getViewportSize().x) / m_applicationModeHandler.getViewportSize().y;
    }

    void Application::onStatePushed()
    {
        m_states.back()->onAttach();
    }
} // namespace Turbo
