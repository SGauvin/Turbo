#include "Turbo/Core/Application.h"
#include <cmath>
#include <glad/glad.h>
#include "Turbo/Core/Log.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/States/State.h"
#include "Turbo/Core/Renderer/BufferLayout.h"

namespace
{
    GLenum getOpenGLType(Turbo::DataType dataType)
    {
        static constexpr GLenum dataTypes[] = {
            GL_FLOAT, // Float
            GL_FLOAT, // Float2
            GL_FLOAT, // Float3
            GL_FLOAT, // Float4
            GL_INT, // Int
            GL_INT, // Int2
            GL_INT, // Int3
            GL_INT, // Int4
            GL_BOOL, // Bool
        };
        return dataTypes[static_cast<std::uint8_t>(dataType)];
    }
}

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

    Application::Application(const WindowAttributes& windowAttributes, InputManager& inputManager)
        : m_window(windowAttributes, inputManager)
        , m_inputManager(inputManager)
    {
        glGenVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f, 1.f,
            0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f, 1.f,
            0.0f, 0.5f, 0.0f, 0.f, 0.f, 1.f, 1.f
        };
        m_vertexBuffer = std::make_unique<VertexBuffer<renderingApi>>(std::span<float>(vertices, sizeof(vertices) / sizeof(float)));

        BufferLayout layout = {
            { DataType::Float3, "position" },
            { DataType::Float4, "position2" },
        };

        for (std::size_t i = 0; i < layout.size(); i++)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, layout[i].getComponentCount(), ::getOpenGLType(layout[i].getDataType()), layout[i].isNormalized(), layout.getStride(), reinterpret_cast<void*>(layout[i].getOffset()));
        }

        std::uint32_t indices[] = {0, 1, 2};
        std::span sheesh = std::span<std::uint32_t>(indices, sizeof(indices) / sizeof(std::uint32_t));
        m_indexBuffer = std::make_unique<IndexBuffer<renderingApi>>(sheesh);

        std::string vertexSource = R"(
            #version 330 core

            layout(location = 0) in vec3 position;
            layout(location = 1) in vec4 color;

            out vec3 v_position;
            out vec4 v_color;

            void main()
            {
                v_position = position;
                v_color = color;
                gl_Position = vec4(position, 1.0);
            }
        )";

        std::string fragmentSource = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            in vec3 v_position;
            in vec4 v_color;

            void main()
            {
                color = vec4(v_position / 2 + 0.5, 1.0);
                color = v_color;
            }
        )";

        m_shader.load(vertexSource, fragmentSource);
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

        m_inputManager.onStateRemove(m_states.back());

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
                glClearColor(0.1f, 0.1f, 0.11f, 1.f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_shader.bind();
                glBindVertexArray(m_vertexArray);
                glDrawElements(GL_TRIANGLES, m_indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

                // Calculate lag for draw interpolation
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
