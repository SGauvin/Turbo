#include <fstream>
#include <iostream>
#include <Turbo/Core/Application/Application.h>
#include <Turbo/Core/Log.h>
#include <Turbo/Core/Renderer/Abstraction/RenderCommand.h>
#include <Turbo/Core/Scene/Components/MeshComponent.h>
#include <Turbo/Core/Scene/Components/TransformComponent.h>
#include <Turbo/Core/Scene/Entity.h>
#include <Turbo/Core/Scene/Scene.h>
#include <Turbo/Core/States/State.h>
#include "Camera.h"

class TriangleLayer : public Turbo::Layer
{
public:
    TriangleLayer(Turbo::Application& application)
        : Layer(application)
        , m_inputContext(m_inputManager.createInputContext())
        , m_camera(m_inputManager)
    {
        m_window.setIsRawMouseEnabled(false);
    }

    void onAttach() override
    {
        std::ifstream file("../assets/to_load.txt");
        if (file)
        {
            std::string line;
            while (std::getline(file, line))
            {
                m_entities = m_scene.loadGlTF(line);
            }
        }
    }

    void onDetach() override {}

    void handleInput() override
    {
        for (auto& entity : m_entities)
        {
            if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::Up))
            {
                entity.getComponent<Turbo::TransformComponent>().rotation.x -= 0.01f;
            }
            if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::Down))
            {
                entity.getComponent<Turbo::TransformComponent>().rotation.x += 0.01f;
            }
            if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::Left))
            {
                entity.getComponent<Turbo::TransformComponent>().rotation.y -= 0.01f;
            }
            if (m_inputManager.isKeyDown(Turbo::Keyboard::Key::Right))
            {
                entity.getComponent<Turbo::TransformComponent>().rotation.y += 0.01f;
            }
        }
    }

    void update() override { m_camera.update(); }

    void draw(float lag = 1.0) override
    {
        Turbo::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1.f});
        Turbo::RenderCommand::clear();
        m_scene.draw(m_camera.getLookAt(), m_camera.getPosition(), m_application.getViewportAspectRatio(), lag);
    }

private:
    Turbo::InputContext* m_inputContext;

    Camera m_camera;
    std::vector<Turbo::Entity> m_entities;
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
    void draw(float /*lag*/) override {}

    void onAttach() override
    {
        m_inputContext = m_inputManager.createInputContext();
        m_inputContext->bindKeyToAction(
            [this]()
            {
                m_window.close();
                return true;
            },
            Turbo::Keyboard::Key::Escape,
            Turbo::Keyboard::Action::Press);

        m_inputContext->bindKeyToAction(
            [this]()
            {
                m_window.setAttributes({m_window.getTitle(),
                                        m_window.getSize(),
                                        m_window.getMode() == Turbo::WindowMode::Bordered ? Turbo::WindowMode::FullScreen : Turbo::WindowMode::Bordered});
                return true;
            },
            Turbo::Keyboard::Key::F11,
            Turbo::Keyboard::Action::Press);

        pushLayer(new TriangleLayer(m_application));
    }
    Turbo::InputContext* m_inputContext = nullptr;
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
