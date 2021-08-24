#include <iostream>
#include <fstream>
#include <Turbo/Core/Application.h>
#include <Turbo/Core/Log.h>
#include <Turbo/Core/Scene/Scene.h>
#include <Turbo/Core/Scene/Components/MeshComponent.h>
#include <Turbo/Core/Scene/Components/TransformComponent.h>
#include <Turbo/Core/States/State.h>
#include <Turbo/Core/Scene/Entity.h>
#include <Turbo/Core/Renderer/Abstraction/RenderCommand.h>

#include "Camera.h"

class TriangleLayer : public Turbo::Layer
{
public:
    TriangleLayer(Turbo::Application& application)
        : Layer(application)
        , m_inputContext(m_inputManager.createInputContext())
        , m_camera(m_inputManager)
    {
        m_window.setIsRawMouseEnabled(true);
    }

    virtual void onAttach()
    {
        std::ifstream file("../assets/to_load.txt");
        if (file)
        {
            std::string line;
            while (std::getline(file, line))
            {
                m_scene.loadGlTF(line);
            }
        }
    }

    virtual void onDetach() {}

    virtual void handleInput()
    {
        if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::Up))
        {
            m_cube.getComponent<Turbo::TransformComponent>().rotation.x -= 0.01f;
        }
        if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::Down))
        {
            m_cube.getComponent<Turbo::TransformComponent>().rotation.x += 0.01f;
        }
        if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::Left))
        {
            m_cube.getComponent<Turbo::TransformComponent>().rotation.y -= 0.01f;
        }
        if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::Right))
        {
            m_cube.getComponent<Turbo::TransformComponent>().rotation.y += 0.01f;
        }
    }

    virtual void update()
    {
        m_camera.update();
    }

    virtual void draw(float lag = 1.0)
    {
        Turbo::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1.f});
        Turbo::RenderCommand::clear();

        const float aspectRatio = static_cast<float>(m_application.getViewportSize().x) / m_application.getViewportSize().y;
        m_scene.draw(m_camera.getLookAt(), m_camera.getPosition(), aspectRatio, lag);
    }

private:
    Turbo::InputContext* m_inputContext;

    Camera m_camera;
    Turbo::Entity m_cube;
    Turbo::Scene m_scene;
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

        pushLayer(new TriangleLayer(m_application));
    }
};

int main()
{
    Turbo::Application app({"Turbo", glm::vec2(2560, 1440), Turbo::WindowMode::Bordered});
    app.setTargetFps(144.f);
    app.setTargetUps(144.f);
    app.getWindow().setIsVSyncEnabled(false);

    app.push<TestState>(app);
    app.start();
}
