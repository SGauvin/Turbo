#include <iostream>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>
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

class TriangleLayer : public Turbo::Layer
{
public:
    TriangleLayer(Turbo::Application& application)
        : Layer(application)
    {
    }

    virtual void onAttach()
    {
        Turbo::BufferLayout layout = {
            { Turbo::DataType::Float3, "position" },
            { Turbo::DataType::Float4, "color" },
        };
        // MESH 1
        {
            float vertices[] = {
                -0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f, 1.f,
                0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f, 1.f,
                0.0f, 0.5f, 0.0f, 0.f, 0.f, 1.f, 1.f
            };
            std::shared_ptr<Turbo::VertexBuffer<Turbo::renderingApi>> vertexBuffer =
                std::make_shared<Turbo::VertexBuffer<Turbo::renderingApi>>(std::span<float>(vertices, sizeof(vertices) / sizeof(float)));
            vertexBuffer->setLayout(layout);

            std::uint32_t indices[] = {0, 1, 2};
            std::shared_ptr<Turbo::IndexBuffer<Turbo::renderingApi>> indexBuffer =
                std::make_shared<Turbo::IndexBuffer<Turbo::renderingApi>>(std::span<std::uint32_t>(indices, sizeof(indices) / sizeof(std::uint32_t)));

            m_vertexArray = std::make_unique<Turbo::VertexArray<Turbo::renderingApi>>();
            m_vertexArray->setVertexBuffer(vertexBuffer);
            m_vertexArray->setIndexBuffer(indexBuffer);
        }

        // MESH 2
        {
            float vertices[] = {
                -0.7f, -0.7f, 0.0f, 0.3f, 1.f, 1.f, 1.f,
                0.7f, -0.7f, 0.0f, 0.3f, 1.f, 1.f, 1.f,
                0.7f, 0.7f, 0.0f, 0.3f, 1.f, 1.f, 1.f,
                -0.7f, 0.7f, 0.0f, 0.3f, 1.f, 1.f, 1.f
            };
            std::shared_ptr<Turbo::VertexBuffer<Turbo::renderingApi>> vertexBuffer = std::make_shared<Turbo::VertexBuffer<Turbo::renderingApi>>(std::span<float>(vertices, sizeof(vertices) / sizeof(float)));
            vertexBuffer->setLayout(layout);

            std::uint32_t indices[] = {0, 1, 2, 2, 3, 0};
            std::shared_ptr<Turbo::IndexBuffer<Turbo::renderingApi>> indexBuffer = std::make_shared<Turbo::IndexBuffer<Turbo::renderingApi>>(std::span<std::uint32_t>(indices, sizeof(indices) / sizeof(std::uint32_t)));

            m_vertexArray2 = std::make_unique<Turbo::VertexArray<Turbo::renderingApi>>();
            m_vertexArray2->setVertexBuffer(vertexBuffer);
            m_vertexArray2->setIndexBuffer(indexBuffer);
        }

        m_shader.loadFromFile("../assets/shader.vert", "../assets/shader.frag");
    }

    virtual void onDetach() {}

    virtual void handleInput() {}
    virtual void update() {}
    virtual void draw(float lag = 1.0)
    {
        Turbo::RenderCommand::setClearColor<Turbo::renderingApi>({0.1f, 0.1f, 0.1f, 1.f});
        Turbo::RenderCommand::clear<Turbo::renderingApi>();

        m_shader.bind();

        m_vertexArray2->bind();
        Turbo::RenderCommand::draw<Turbo::renderingApi>(m_vertexArray2.get());

        m_vertexArray->bind();
        Turbo::RenderCommand::draw<Turbo::renderingApi>(m_vertexArray.get());
    }

private:
    Turbo::Shader<Turbo::renderingApi> m_shader;
    
    std::unique_ptr<Turbo::VertexArray<Turbo::renderingApi>> m_vertexArray;
    std::unique_ptr<Turbo::VertexArray<Turbo::renderingApi>> m_vertexArray2;
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
    Turbo::Application app({"Turbo", glm::vec2(1280, 720), Turbo::WindowMode::Bordered});
    app.setTargetFps(144.f);
    app.setTargetUps(60.f);
    app.push(new TestState(app));
    app.start();
}
