#include "Turbo/Core/Window/OpenGLWindow.h"
#include "Turbo/Core/Log.h"
#include <glad/glad.h>

namespace
{
    bool checkWindow(GLFWwindow* window)
    {
        if (window == nullptr)
        {
            TURBO_ENGINE_ERROR("Window does not exist.");
            return false;
        }
        return true;
    }
} // namespace

namespace Turbo
{
    Window<RenderingApi::OpenGL>::Window(const WindowAttributes& windowAttributes, InputManager& inputManager)
        : m_inputManager(inputManager)
    {
        create(windowAttributes);
        setIsRawMouseEnabled(m_isRawMouseEnabled);
    }

    Window<RenderingApi::OpenGL>::~Window() { destroy(); }

    void Window<RenderingApi::OpenGL>::setAttributes(const WindowAttributes& windowAttributes)
    {
        if (m_mode == windowAttributes.mode)
        {
            return;
        }

        m_size = {windowAttributes.size.x, windowAttributes.size.y};
        m_mode = windowAttributes.mode;

        if (m_windowTitle != windowAttributes.title)
        {
            m_windowTitle = windowAttributes.title;
            glfwSetWindowTitle(m_window, m_windowTitle.c_str());
        }

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        switch(m_mode)
        {
        case WindowMode::Bordered:
            glfwSetWindowMonitor(m_window, nullptr, m_windowPosition.x, m_windowPosition.y, m_size.x, m_size.y, GLFW_DONT_CARE);
            break;
        case WindowMode::FullScreen:
            glfwGetWindowPos(m_window, &m_windowPosition.x, &m_windowPosition.y);
            glfwSetWindowMonitor(m_window, monitor, 0, 0, m_size.x, m_size.y, GLFW_DONT_CARE);
            break;
        }
    }

    void Window<RenderingApi::OpenGL>::destroy()
    {
        if (m_window == nullptr)
        {
            return;
        }
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
        TURBO_ENGINE_INFO("Window terminated");
    }

    void Window<RenderingApi::OpenGL>::close() { m_shouldClose = true; }

    void Window<RenderingApi::OpenGL>::swapBuffers() { m_context->swapBuffers(); }

    void Window<RenderingApi::OpenGL>::processEvents()
    {
        m_inputManager.onPollEvents();
        glfwPollEvents();
    }

    glm::uvec2 Window<RenderingApi::OpenGL>::getSize() const { return m_size; }

    WindowMode Window<RenderingApi::OpenGL>::getMode() const { return m_mode; }

    const std::string& Window<RenderingApi::OpenGL>::getTitle() const { return m_windowTitle; }

    bool Window<RenderingApi::OpenGL>::isOpen() const { return !glfwWindowShouldClose(m_window) && !m_shouldClose; }

    bool Window<RenderingApi::OpenGL>::isRawMouseAvailable() const { return glfwRawMouseMotionSupported(); }

    GLFWwindow* Window<RenderingApi::OpenGL>::getHandle() const { return m_window; }

    void Window<RenderingApi::OpenGL>::setIsVSyncEnabled(bool isVsyncEnabled)
    {
        m_isVsyncEnabled = isVsyncEnabled;
        if (!checkWindow(m_window))
            return;

        GLFWwindow* oldContext = glfwGetCurrentContext();
        if (oldContext == m_window)
        {
            glfwSwapInterval(m_isVsyncEnabled ? 1 : 0);
        }
        else
        {
            glfwMakeContextCurrent(m_window);
            glfwSwapInterval(m_isVsyncEnabled ? 1 : 0);
            glfwMakeContextCurrent(oldContext);
        }
    }

    void Window<RenderingApi::OpenGL>::setIsRawMouseEnabled(bool isRawMouseEnabled)
    {
        if (isRawMouseEnabled && !isRawMouseAvailable())
        {
            TURBO_ENGINE_ERROR("Could not enable raw mouse input. Be sure to check if it is available before enabling.");
            return;
        }
        m_isRawMouseEnabled = isRawMouseEnabled;

        if (!checkWindow(m_window))
            return;

        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, isRawMouseEnabled);
    }

    void Window<RenderingApi::OpenGL>::setIsResizable(bool isResizable)
    {
        m_isResizable = isResizable;

        if (!checkWindow(m_window))
            return;

        glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, isResizable);
    }

    void Window<RenderingApi::OpenGL>::create(const WindowAttributes& windowAttributes)
    {
        destroy();
        glfwInit();

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        m_mode = windowAttributes.mode;
        m_size = {windowAttributes.size.x, windowAttributes.size.y};
        m_windowTitle = windowAttributes.title;

        switch(m_mode)
        {
        case WindowMode::Bordered:
            m_window = glfwCreateWindow(windowAttributes.size.x, windowAttributes.size.y, windowAttributes.title.c_str(), nullptr, nullptr);
            glfwGetWindowPos(m_window, &m_windowPosition.x, &m_windowPosition.y);
            break;
        case WindowMode::FullScreen:
            m_window = glfwCreateWindow(windowAttributes.size.x, windowAttributes.size.y, windowAttributes.title.c_str(), monitor, nullptr);
            break;
        }

        if (m_window == nullptr)
        {
            TURBO_ENGINE_ERROR("Error creating the window");
        }
        else
        {
            TURBO_ENGINE_INFO("Window created");
        }

        m_context = std::make_unique<OpenGLContext>(m_window);
        m_context->init();

        setIsVSyncEnabled(m_isVsyncEnabled);

        setCallbacks();

        setIsResizable(m_isResizable);

        glfwGetCursorPos(m_window, &m_inputManager.m_mousePosition.x, &m_inputManager.m_mousePosition.y);
    }

    void Window<RenderingApi::OpenGL>::setCallbacks()
    {
        glfwSetWindowUserPointer(m_window, this);

        // Window resize
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, std::int32_t width, std::int32_t height) {
            static_cast<Window<RenderingApi::OpenGL>*>(glfwGetWindowUserPointer(window))->onWindowResize({width, height});
        });

        // Keyboard
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods) {
            static_cast<Window<RenderingApi::OpenGL>*>(glfwGetWindowUserPointer(window))
                ->onKeyEvent(static_cast<Keyboard::Key>(key), scancode, static_cast<Keyboard::Action>(action), static_cast<std::uint8_t>(mods));
        });

        glfwSetCharCallback(
            m_window, [](GLFWwindow* window, std::uint32_t character) { static_cast<Window<RenderingApi::OpenGL>*>(glfwGetWindowUserPointer(window))->onTextEnterEvent(character); });

        // Cursor position
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
            static_cast<Window<RenderingApi::OpenGL>*>(glfwGetWindowUserPointer(window))->onMouseMove({xpos, ypos});
        });

        // Mouse buttons
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, std::int32_t button, std::int32_t action, std::int32_t mods) {
            static_cast<Window<RenderingApi::OpenGL>*>(glfwGetWindowUserPointer(window))
                ->onMouseButtonEvent(static_cast<Mouse::Button>(button), static_cast<Mouse::Action>(action), static_cast<std::uint8_t>(mods));
        });

        // Mouse scroll
        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double offsetX, double offsetY) {
            static_cast<Window<RenderingApi::OpenGL>*>(glfwGetWindowUserPointer(window))->onMouseScrollEvent({offsetX, offsetY});
        });
    }

    void Window<RenderingApi::OpenGL>::onWindowResize(glm::uvec2 windowSize)
    {
        m_size = windowSize;
        glViewport(0, 0, m_size.x, m_size.y);
    }

    void Window<RenderingApi::OpenGL>::onKeyEvent(Keyboard::Key key, std::int32_t /*scancode*/, Keyboard::Action action, std::uint8_t mods)
    {
        m_inputManager.onKeyboardEvent({key, action, mods});
    }

    void Window<RenderingApi::OpenGL>::onTextEnterEvent(std::uint32_t character) { m_inputManager.onTextEnterEvent(character); }

    void Window<RenderingApi::OpenGL>::onMouseScrollEvent(const glm::dvec2& delta) { m_inputManager.onMouseScrollEvent({delta}); }

    void Window<RenderingApi::OpenGL>::onMouseMove(const glm::dvec2& mousePosition)
    {
        m_inputManager.onMouseMoveEvent(Mouse::MoveEvent{mousePosition, mousePosition - m_inputManager.getMousePosition()});
    }

    void Window<RenderingApi::OpenGL>::onMouseButtonEvent(Mouse::Button button, Mouse::Action action, std::uint8_t mods)
    {
        m_inputManager.onMouseButtonEvent({button, action, m_inputManager.getMousePosition(), mods});
    }
} // namespace Turbo