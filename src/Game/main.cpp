#include <iostream>
// #include <Turbo/Test.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>
#include <Turbo/Core/Init.h>
#include <Turbo/Core/Log.h>
#include <Turbo/Core/Window.h>

void test()
{
    TURBO_INFO("ye!");
    // Turbo::Log::info("ye!");
}

void test2(bool test)
{
    TURBO_INFO("State: {}", test);
    TURBO_WARNING("State: {}", test);
    TURBO_ERROR("State: {}", test);
    // Turbo::Log::info("State: {}", test);
}

int main()
{
    Turbo::init();

    // Turbo::Log::engineInfo("Bonjour!\n");
    Turbo::InputManager inputManager;

    Turbo::Window window({"Test", glm::vec2(400, 400), Turbo::Window::Mode::Bordered}, inputManager);
    Turbo::InputContext& inputContext = inputManager.createInputContext();

    inputContext.bindKeyToState(test2, Turbo::Keyboard::Key::W);
    inputContext.bindKeyToAction([&window]() { window.close(); }, Turbo::Keyboard::Key::Escape, Turbo::Keyboard::Action::Press);
    inputContext.bindKeyToAction(
        [&window]() {
            window.create({"Test2", glm::vec2(500, 500), Turbo::Window::Mode::Bordered});
        },
        Turbo::Keyboard::Key::E,
        Turbo::Keyboard::Action::Press);

    while (window.isOpen())
    {
        window.processEvents();
    }
}