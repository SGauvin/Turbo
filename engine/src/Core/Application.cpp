#include "Turbo/Core/Application.h"

#include <cmath>

#include "Turbo/Core/Log.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/RenderCommand.h"
#include "Turbo/Core/Renderer/BufferLayout.h"
#include "Turbo/Core/States/State.h"
#include "Turbo/Core/Window/OpenGLWindow.h"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

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
    {
        if constexpr (isEditorEnabled)
        {
            m_viewportFrameBuffer = std::make_unique<FrameBuffer<renderingApi>>(glm::ivec2(800, 600));
        }
    }

    Application::~Application()
    {
        for (const auto& state : m_states)
        {
            delete state;
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
        std::chrono::duration<double> m_timePerUpdate = std::chrono::nanoseconds(static_cast<std::uint32_t>(std::round(1000000000.f / ups)));
    }

    void Application::setTargetFps(double fps)
    {
        std::chrono::duration<double> m_timePerDraw = std::chrono::nanoseconds(static_cast<std::uint32_t>(std::round(1000000000.f / fps)));
    }

    void Application::start()
    {
        Clock clock;

        RenderCommand::init();
        while (m_window.isOpen() && !m_states.empty()) [[likely]]
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
                if (getViewportSize().x > 0.f)
                {
                    RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1.f});
                    RenderCommand::clear();

                    // Calculate lag for draw interpolation
                    float lag = static_cast<float>(m_updateLag / m_timePerUpdate);

                    if constexpr (isEditorEnabled)
                    {
                        RenderCommand::beginViewport(m_viewportFrameBuffer.get());
                    }

                    m_states.back()->draw(lag);
                    for (const auto& layer : m_states.back()->m_layers)
                    {
                        if (layer->enableDraw)
                        {
                            layer->draw(lag);
                        }
                    }

                    if constexpr (isEditorEnabled)
                    {
                        RenderCommand::endViewport(m_viewportFrameBuffer.get(), m_window.getSize());
                    }

                    m_window.swapBuffers();
                }

                m_drawLag -= std::floor(m_drawLag / m_timePerDraw) * m_timePerDraw;
            }
        }
    }

    glm::uvec2 Application::getViewportSize() const
    {
        if constexpr (isEditorEnabled)
        {
            return m_viewportFrameBuffer->getSize();
        }
        return m_window.getSize();
    }

    
    void Application::onStatePushed()
    {
        m_states.back()->onAttach();
    }
} // namespace Turbo
