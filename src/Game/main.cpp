#include <iostream>
// #include <Turbo/Test.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>
#include <Turbo/Core/Application.h>
#include <Turbo/Core/Init.h>
#include <Turbo/Core/Layers/ImGuiLayer.h>
#include <Turbo/Core/Log.h>
#include <Turbo/Core/States/State.h>
#include <Turbo/Core/Window.h>

// class Layer1 : public Turbo::Layer
// {
// public:
//     Layer1(Turbo::Application& application)
//         : Turbo::Layer(application)
//         , m_inputContext(m_inputManager.createInputContext())
//     {
//         m_inputContext->bindKeyToAction(this, &onKeyEscapePress, Turbo::Keyboard::Key::Escape, Turbo::Keyboard::Action::Press);
//     }

//     bool onKeyEscapePress()
//     {
//         TURBO_INFO("Layer1: Esc pressed.");
//         return true;
//     }

//     void handleInput() override {}
//     void update() override {}
//     void draw(float lag = 1.0) override {}

// private:
//     Turbo::InputContext* m_inputContext;
// };

// class Layer2 : public Turbo::Layer
// {
// public:
//     Layer2(Turbo::Application& application)
//         : Turbo::Layer(application)
//         , m_inputContext(m_inputManager.createInputContext())
//     {
//         m_inputContext->bindKeyToAction(this, &onKeyEscapePress, Turbo::Keyboard::Key::Escape, Turbo::Keyboard::Action::Press);
//     }

//     bool onKeyEscapePress()
//     {
//         TURBO_INFO("Layer2: Esc pressed.");
//         return true;
//     }

//     void handleInput() override {}
//     void update() override {}
//     void draw(float lag = 1.0) override {}

// private:
//     Turbo::InputContext* m_inputContext;
// };

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
        auto* inputContext = m_inputManager.createInputContext();
        inputContext->bindKeyToAction(
            [this]() {
                m_window.close();
                return true;
            },
            Turbo::Keyboard::Key::Escape,
            Turbo::Keyboard::Action::Press);

        pushLayer(new Turbo::ImGuiLayer(m_application));
    }
};

int main()
{
    Turbo::init();

    Turbo::InputManager inputManager;
    Turbo::Window window({"Test", glm::vec2(1920, 1080), Turbo::Window::Mode::Bordered}, inputManager);

    Turbo::Application app(window, inputManager);
    app.push(new TestState(app));
    app.start();
}