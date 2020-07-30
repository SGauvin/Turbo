#ifndef INCLUDED_TURBO_WINDOW_H
#define INCLUDED_TURBO_WINDOW_H

#include <cstdint>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Turbo/Core/Input/InputManager.h"

namespace Turbo
{
    class Window
    {
    public:
        enum class Mode : std::uint8_t
        {
            Bordered = 0,
            Borderless,
            FullScreen
        };

        struct Attributes
        {
            std::string title = "";
            glm::uvec2 size = {300, 300};
            Mode mode = Mode::Bordered;
        };

        Window(const Attributes& windowAttributes, InputManager& inputManager);
        Window(const Window& other) = delete;
        Window(const Window&& other) = delete;
        Window& operator=(const Window& other) = delete;
        Window& operator=(const Window&& other) = delete;
        ~Window();

        // Functions
        void create(const Attributes& windowAttributes);
        void destroy();
        void close();
        void clear();
        void swapBuffers();

        void processEvents();

        // Getters
        glm::uvec2 getSize() const { return m_size; }
        Mode getMode() const { return m_mode; }
        bool isOpen() const;
        bool isRawMouseAvailable() const;

        // Setters
        void setIsVSyncEnabled(bool isVsynEnabled);
        void setIsRawMouseEnabled(bool isRawMouseEnabled);
        void setIsResizable(bool isResizable);

        // Static functions
        static void init();

    private:
        void setCallbacks();

        void onWindowResize(glm::uvec2 windowSize);
        void onKeyEvent(Keyboard::Key key, std::int32_t scancode, Keyboard::Action action, std::uint8_t mods);
        void onMouseMove(glm::dvec2 mousePosition);
        void onMouseButtonEvent(Mouse::Button button, Mouse::Action action, std::uint8_t mods);

        GLFWwindow* m_window = nullptr;
        InputManager& m_inputManager;

        // Attributes
        glm::uvec2 m_size = {0, 0};
        Mode m_mode;

        // Settings
        bool m_isResizable = true;
        bool m_isVsyncEnabled = false;
        bool m_isRawMouseEnabled = false;
        bool m_shouldClose = false;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_WINDOW_H
