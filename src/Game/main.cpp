#include <iostream>
// #include <Turbo/Test.h>
#include <iostream>
#include <Turbo/Core/Init.h>
#include <Turbo/Core/Log.h>
#include <Turbo/Core/Window.h>

#include <chrono>
#include <thread>
#include <utility>

void test()
{
    Turbo::Log::info("ye!");
}

void test2(bool test)
{
    Turbo::Log::info("State: {}", test);
}

int main()
{
    Turbo::init();

    Turbo::InputManager inputManager;

    Turbo::Window window({"Test", glm::vec2(400, 400)}, inputManager);

    Turbo::InputContext& inputContext = inputManager.createInputContext();
    inputContext.bindKeyToAction(test, Turbo::Keyboard::Key::A, Turbo::Keyboard::Action::Press, Turbo::Keyboard::Modifier::Shift | Turbo::Keyboard::Modifier::Control);
    inputContext.bindKeyToState(test2, Turbo::Keyboard::Key::W, Turbo::Keyboard::Modifier::Shift);
    inputContext.bindKeyToRange([](float test) { Turbo::Log::info("float: {}", test); }, Turbo::Keyboard::Key::Up, Turbo::InputContext::Direction::Positive);
    inputContext.bindKeyToRange([](float test) { Turbo::Log::info("float: {}", test); }, Turbo::Keyboard::Key::Down, Turbo::InputContext::Direction::Negative);

    inputContext.bindKeysToRange([](float test) { Turbo::Log::info("float: {}", test); }, Turbo::Keyboard::Key::Left, Turbo::Keyboard::Key::Right, Turbo::Keyboard::Modifier::Control);

    while (window.isOpen())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(17));
        window.processEvents();

        Turbo::Log::info("F key: {}", inputManager.isKeyDown(Turbo::Keyboard::Key::F));
    }
}