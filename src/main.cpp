#include <iostream>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>
#include <Turbo/Core/Scene/Components/MeshComponent.h>
#include <Turbo/Core/Application.h>
#include <Turbo/Core/Layers/ImGuiLayer.h>
#include <Turbo/Core/Log.h>
#include <Turbo/Core/States/State.h>
#include <Turbo/Core/Renderer/Abstraction/RenderCommand.h>
#include <Turbo/Core/Renderer/Abstraction/Shader.h>
#include <Turbo/Core/Renderer/Abstraction/VertexArray.h>
#include <Turbo/Core/Renderer/Abstraction/VertexBuffer.h>
#include <Turbo/Core/Renderer/Abstraction/IndexBuffer.h>
#include <Turbo/Core/Renderer/BufferLayout.h>
#include <glm/gtx/transform.hpp>

class TriangleLayer : public Turbo::Layer
{
public:
    TriangleLayer(Turbo::Application& application)
        : Layer(application)
    {
        m_shader.loadFromFile("../assets/shader.vert", "../assets/shader.frag");
        m_shaderLightCube.loadFromFile("../assets/shader.vert", "../assets/shader2.frag");
        m_window.setIsRawMouseEnabled(true);
    }

    virtual void onAttach()
    {
        m_inputContext = m_inputManager.createInputContext();

        m_inputContext->bindMouseMoveEvents([this](const Turbo::Mouse::MoveEvent& moveEvent)
        {
            m_cameraYaw += moveEvent.movement.x / 1000.f;
            m_cameraPitch -= moveEvent.movement.y / 1000.f;
            return true;
        });
    }

    virtual void onDetach() {}

    virtual void handleInput()
    {
        if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::Enter))
        {
            m_cubeAngle -= 1.f;
        }

        glm::vec3 cameraFront = glm::vec3(
            glm::cos(m_cameraYaw) * glm::cos(m_cameraPitch),
            glm::sin(m_cameraPitch),
            glm::sin(m_cameraYaw) * glm::cos(m_cameraPitch)
        );

        static const float sensitivity = 0.05f;

        if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::W))
        {
            m_cameraPosition += cameraFront * sensitivity;
        }
        if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::S))
        {
            m_cameraPosition -= cameraFront * sensitivity;
        }
        if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::A))
        {
            m_cameraPosition -= glm::normalize(glm::cross(cameraFront, m_cameraUp)) * sensitivity;
        }
        if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::D))
        {
            m_cameraPosition += glm::normalize(glm::cross(cameraFront, m_cameraUp)) * sensitivity;
        }
    }

    virtual void update()
    {
    }

    virtual void draw(float lag = 1.0)
    {
        Turbo::RenderCommand::setClearColor<Turbo::renderingApi>({0.1f, 0.1f, 0.1f, 1.f});
        Turbo::RenderCommand::clear<Turbo::renderingApi>();

        {
            m_shader.bind();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(m_cubeAngle), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(30.f), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec3 cameraFront = glm::vec3(
                glm::cos(m_cameraYaw) * glm::cos(m_cameraPitch),
                glm::sin(m_cameraPitch),
                glm::sin(m_cameraYaw) * glm::cos(m_cameraPitch)
            );
            glm::mat4 view = glm::lookAt(m_cameraPosition, m_cameraPosition + cameraFront, m_cameraUp);

            glm::mat4 projection;
            projection = glm::perspective(glm::radians(45.0f), static_cast<float>(m_application.getViewportSize().x) / static_cast<float>(m_application.getViewportSize().y), 0.1f, 100.0f);
        
            m_shader.setMatrix4("model", model);
            m_shader.setMatrix4("view", view);
            m_shader.setMatrix4("projection", projection);
            m_shader.setFloat3("lightPosition", m_lightPos);
            m_shader.setFloat3("cameraPosition", m_cameraPosition);

            m_mesh.getVertexArray()->bind();
            Turbo::RenderCommand::draw<Turbo::renderingApi>(m_mesh.getVertexArray());
        }

        {
            m_shaderLightCube.bind();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_lightPos);
            model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));

            glm::vec3 cameraFront = glm::vec3(
                glm::cos(m_cameraYaw) * glm::cos(m_cameraPitch),
                glm::sin(m_cameraPitch),
                glm::sin(m_cameraYaw) * glm::cos(m_cameraPitch)
            );
            glm::mat4 view = glm::lookAt(m_cameraPosition, m_cameraPosition + cameraFront, m_cameraUp);

            glm::mat4 projection;
            projection = glm::perspective(glm::radians(45.0f), static_cast<float>(m_application.getViewportSize().x) / static_cast<float>(m_application.getViewportSize().y), 0.1f, 100.0f);
        
            m_shaderLightCube.setMatrix4("model", model);
            m_shaderLightCube.setMatrix4("view", view);
            m_shaderLightCube.setMatrix4("projection", projection);

            m_meshLightCube.getVertexArray()->bind();
            Turbo::RenderCommand::draw<Turbo::renderingApi>(m_meshLightCube.getVertexArray());
        }
    }

private:
    Turbo::Shader<Turbo::renderingApi> m_shader;
    Turbo::Shader<Turbo::renderingApi> m_shaderLightCube;

    Turbo::MeshComponent m_mesh;
    Turbo::MeshComponent m_meshLightCube;

    float m_cubeAngle = 0.f;

    glm::vec3 m_cameraPosition = glm::vec3(0.f, 0.f, 3.f);
    glm::vec3 m_cameraUp = glm::vec3(0.f, 1.f, 0.f);
    float m_cameraYaw = 3.1416 * 1.5;
    float m_cameraPitch = 0;

    glm::vec3 m_lightPos = glm::vec3(2.f, 1.2f, 4.f);
    Turbo::InputContext* m_inputContext = nullptr;
};

class TestState : public Turbo::State
{
public:
    TestState(Turbo::Application& application)
        : Turbo::State(application)
    {
    }

private:
    void handleInput() override {}
    void update() override {}
    void draw(float lag = 1.0) override {}

    void onAttach() override
    {
        static auto* inputContext = m_inputManager.createInputContext();
        inputContext->bindKeyToAction(
            [this]() {
                m_window.close();
                return true;
            },
            Turbo::Keyboard::Key::Escape,
            Turbo::Keyboard::Action::Press);

        inputContext->bindKeyToAction(
            [this]() {
                m_window.setAttributes({m_window.getTitle(),
                                        m_window.getSize(),
                                        m_window.getMode() == Turbo::WindowMode::Bordered ? Turbo::WindowMode::FullScreen : Turbo::WindowMode::Bordered});
                return true;
            },
            Turbo::Keyboard::Key::F11,
            Turbo::Keyboard::Action::Press);

        static Turbo::InputHandle inputHandleAction;
        static Turbo::InputHandle inputHandleBind;

        static auto action = []() {
            TURBO_INFO("Action!");
            return true;
        };

        static auto bindAction = [](const Turbo::Keyboard::KeyEvent& event) {
            TURBO_INFO("Bound to {}", static_cast<char>(event.key));
            inputHandleBind.unbind();
            inputHandleAction = inputContext->bindKeyToAction(action, event.key, Turbo::Keyboard::Action::Press, event.modifiers);
            return true;
        };

        static auto unbindAction = []() {
            TURBO_INFO("Press a key to bind it to action!");
            inputHandleAction.unbind();
            inputHandleBind = inputContext->bindKeyReleaseEvents(bindAction);
            return true;
        };

        inputContext->bindKeyToAction(unbindAction, Turbo::Keyboard::Key::Insert, Turbo::Keyboard::Action::Release);

        pushLayer(new TriangleLayer(m_application));
        pushLayer(new Turbo::ImGuiLayer(m_application));
    }
};

int main()
{
    Turbo::Application app({"Turbo", glm::vec2(2560, 1440), Turbo::WindowMode::FullScreen});
    app.setTargetFps(144.f);
    app.setTargetUps(60.f);
    app.push(new TestState(app));
    app.start();
}
