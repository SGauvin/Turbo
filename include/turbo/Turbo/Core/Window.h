#ifndef TURBO_WINDOW_H
#define TURBO_WINDOW_H

#include <cstdint>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Turbo/Core/Input/InputManager.h"

namespace Turbo
{
    class Window
    {
    public:
        struct Attributes
        {
            std::string title;
            glm::uvec2 size;
        };

        Window(const Attributes& windowAttributes, InputManager& inputManager);
        ~Window();

        static void init();

        void processEvents();

        bool isOpen() const;

    private:
        void onKeyAction(Keyboard::Key key, std::int32_t scancode, Keyboard::Action action, std::uint8_t mods);

        GLFWwindow* m_window;
        InputManager& m_inputManager;
    };
} // namespace Turbo

#endif // TURBO_WINDOW_H
