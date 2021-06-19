#ifndef INCLUDED_TURBO_OPENGLWINDOW_H
#define INCLUDED_TURBO_OPENGLWINDOW_H

#include <memory>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Turbo/Core/Input/InputManager.h"
#include "Turbo/Core/Renderer/OpenGL/OpenGLContext.h"
#include "Turbo/Core/Window/Window.h"

namespace Turbo
{
    
    class OpenGLWindow
    {
    public:
        OpenGLWindow(const WindowAttributes& windowAttributes, InputManager& inputManager);
        OpenGLWindow(const OpenGLWindow& other) = delete;
        OpenGLWindow(const OpenGLWindow&& other) = delete;
        OpenGLWindow& operator=(const OpenGLWindow& other) = delete;
        OpenGLWindow& operator=(const OpenGLWindow&& other) = delete;
        ~OpenGLWindow();

        // Functions
        void setAttributes(const WindowAttributes& windowAttributes);
        void destroy();
        void close();
        void swapBuffers();

        void processEvents();

        // Getters
        glm::uvec2 getSize() const;
        WindowMode getMode() const;
        const std::string& getTitle() const;
        bool isOpen() const;
        bool isRawMouseAvailable() const;
        GLFWwindow* getWindow() const { return m_window; }

        // Setters
        void setIsVSyncEnabled(bool isVsynEnabled);
        void setIsRawMouseEnabled(bool isRawMouseEnabled);
        void setIsResizable(bool isResizable);

    private:
        void create(const WindowAttributes& windowAttributes);
        void setCallbacks();

        void onWindowResize(glm::uvec2 windowSize);
        void onKeyEvent(Keyboard::Key key, std::int32_t scancode, Keyboard::Action action, std::uint8_t mods);
        void onTextEnterEvent(std::uint32_t character);
        void onMouseMove(const glm::dvec2& mousePosition);
        void onMouseScrollEvent(const glm::dvec2& delta);
        void onMouseButtonEvent(Mouse::Button button, Mouse::Action action, std::uint8_t mods);

        GLFWwindow* m_window = nullptr;
        InputManager& m_inputManager;
        std::unique_ptr<OpenGLContext> m_context;

        // Attributes
        glm::uvec2 m_size = {0, 0};
        WindowMode m_mode;
        std::string m_windowTitle;

        // Settings
        bool m_isResizable = true;
        bool m_isVsyncEnabled = false;
        bool m_isRawMouseEnabled = false;
        bool m_shouldClose = false;

        // Memory
        glm::ivec2 m_windowPosition = {0, 0};
    };

} // namespace Turbo

#endif // INCLUDED_TURBO_OPENGLWINDOW_H
