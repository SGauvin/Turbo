#include <glad/glad.h>
#include "Turbo/Core/Application.h"
#include <cmath>
#include "Turbo/Core/Log.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/States/State.h"
#include "Turbo/Core/Renderer/BufferLayout.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Turbo/Core/Window/OpenGLWindow.h"
#include "imgui/backends/imgui_impl_glfw.h"

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

    Application::Application(const WindowAttributes& windowAttributes)
        : m_inputManager()
        , m_window(windowAttributes, m_inputManager)
    {
        BufferLayout layout = {
            { DataType::Float3, "position" },
            { DataType::Float4, "color" },
        };
        // MESH 1
        {
            glGenVertexArrays(1, &m_vertexArray);
            glBindVertexArray(m_vertexArray);

            float vertices[] = {
                -0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f, 1.f,
                0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f, 1.f,
                0.0f, 0.5f, 0.0f, 0.f, 0.f, 1.f, 1.f
            };
            m_vertexBuffer = std::make_unique<VertexBuffer<renderingApi>>(std::span<float>(vertices, sizeof(vertices) / sizeof(float)), layout);

            for (std::size_t i = 0; i < layout.size(); i++)
            {
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i, layout[i].getComponentCount(), ::getOpenGLType(layout[i].getDataType()), layout[i].isNormalized(), layout.getStride(), reinterpret_cast<void*>(layout[i].getOffset()));
            }

            std::uint32_t indices[] = {0, 1, 2};
            m_indexBuffer = std::make_unique<IndexBuffer<renderingApi>>(std::span<std::uint32_t>(indices, sizeof(indices) / sizeof(std::uint32_t)));
        }

        // MESH 2
        {
            glGenVertexArrays(1, &m_vertexArray2);
            glBindVertexArray(m_vertexArray2);

            float vertices[] = {
                -0.5f, -0.5f, 0.0f, 1.f, 1.f, 0.f, 1.f,
                0.5f, -0.5f, 0.0f, 1.f, 1.f, 0.f, 1.f,
                0.5f, 0.5f, 0.0f, 1.f, 1.f, 0.f, 1.f,
                -0.5f, 0.5f, 0.0f, 1.f, 1.f, 0.f, 1.f
            };
            m_vertexBuffer2 = std::make_unique<VertexBuffer<renderingApi>>(std::span<float>(vertices, sizeof(vertices) / sizeof(float)), layout);

            std::uint32_t indices[] = {0, 1, 2, 2, 3, 0};
            m_indexBuffer2 = std::make_unique<IndexBuffer<renderingApi>>(std::span<std::uint32_t>(indices, sizeof(indices) / sizeof(std::uint32_t)));
        }

        for (std::size_t i = 0; i < layout.size(); i++)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, layout[i].getComponentCount(), ::getOpenGLType(layout[i].getDataType()), layout[i].isNormalized(), layout.getStride(), reinterpret_cast<void*>(layout[i].getOffset()));
        }

        // SHADERS

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

        m_frameBuffer = std::make_unique<FrameBuffer<renderingApi>>(glm::ivec2{800, 600});
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

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // VIEWPORT

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
                ImGui::Begin("GameWindow");
                static ImVec2 viewportSize = ImVec2(0, 0);
                ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
                if (viewportSize.x != currentViewportSize.x || viewportSize.y != currentViewportSize.y)
                {
                    viewportSize = currentViewportSize;
                    m_frameBuffer->resize(glm::ivec2(viewportSize.x, viewportSize.y));
                }

                m_frameBuffer->bind();
                glViewport(0, 0, viewportSize.x, viewportSize.y);
                glClearColor(0.1f, 0.1f, 0.11f, 1.f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_shader.bind();
        
                glBindVertexArray(m_vertexArray2);
                glDrawElements(GL_TRIANGLES, m_indexBuffer2->getCount(), GL_UNSIGNED_INT, nullptr);

                glBindVertexArray(m_vertexArray);
                glDrawElements(GL_TRIANGLES, m_indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                m_frameBuffer->unbind();

                ImGui::Image(reinterpret_cast<void*>(m_frameBuffer->getTexture()), viewportSize, ImVec2(0, 1), ImVec2(1, 0));
                ImGui::End();
                ImGui::PopStyleVar();

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


                ImGuiIO& io = ImGui::GetIO();
                io.DisplaySize = {static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y)};
                io.DeltaTime = (1.0f / 60.0f);

                if (io.ConfigFlags | ImGuiConfigFlags_ViewportsEnable)
                {
                    GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    glfwMakeContextCurrent(backupCurrentContext);
                }

                m_window.swapBuffers();

                m_drawLag -= std::floor(m_drawLag / m_timePerDraw) * m_timePerDraw;
            }
        }
    }
} // namespace Turbo
