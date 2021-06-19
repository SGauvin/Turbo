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
    OpenGLWindow::OpenGLWindow(const WindowAttributes& windowAttributes, InputManager& inputManager)
        : m_inputManager(inputManager)
    {
        create(windowAttributes);
        setIsRawMouseEnabled(m_isRawMouseEnabled);
    }

    OpenGLWindow::~OpenGLWindow() { destroy(); }

    void OpenGLWindow::setAttributes(const WindowAttributes& windowAttributes)
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

    void OpenGLWindow::destroy()
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

    void OpenGLWindow::close() { m_shouldClose = true; }

    void OpenGLWindow::swapBuffers() { m_context->swapBuffers(); }

    void OpenGLWindow::processEvents()
    {
        m_inputManager.onPollEvents();
        glfwPollEvents();
    }

    glm::uvec2 OpenGLWindow::getSize() const { return m_size; }

    WindowMode OpenGLWindow::getMode() const { return m_mode; }

    const std::string& OpenGLWindow::getTitle() const { return m_windowTitle; }

    bool OpenGLWindow::isOpen() const { return !glfwWindowShouldClose(m_window) && !m_shouldClose; }

    bool OpenGLWindow::isRawMouseAvailable() const { return glfwRawMouseMotionSupported(); }

    void OpenGLWindow::setIsVSyncEnabled(bool isVsyncEnabled)
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

    void OpenGLWindow::setIsRawMouseEnabled(bool isRawMouseEnabled)
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

    void OpenGLWindow::setIsResizable(bool isResizable)
    {
        m_isResizable = isResizable;

        if (!checkWindow(m_window))
            return;

        glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, isResizable);
    }

    void OpenGLWindow::create(const WindowAttributes& windowAttributes)
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

    void OpenGLWindow::setCallbacks()
    {
        glfwSetWindowUserPointer(m_window, this);

        // Window resize
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, std::int32_t width, std::int32_t height) {
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window))->onWindowResize({width, height});
        });

        // Keyboard
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods) {
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window))
                ->onKeyEvent(static_cast<Keyboard::Key>(key), scancode, static_cast<Keyboard::Action>(action), static_cast<std::uint8_t>(mods));
        });

        glfwSetCharCallback(
            m_window, [](GLFWwindow* window, std::uint32_t character) { static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window))->onTextEnterEvent(character); });

        // Cursor position
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window))->onMouseMove({xpos, ypos});
        });

        // Mouse buttons
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, std::int32_t button, std::int32_t action, std::int32_t mods) {
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window))
                ->onMouseButtonEvent(static_cast<Mouse::Button>(button), static_cast<Mouse::Action>(action), static_cast<std::uint8_t>(mods));
        });

        // Mouse scroll
        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double offsetX, double offsetY) {
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window))->onMouseScrollEvent({offsetX, offsetY});
        });
    }

    void OpenGLWindow::onWindowResize(glm::uvec2 windowSize)
    {
        m_size = windowSize;
        glViewport(0, 0, m_size.x, m_size.y);
    }

    void OpenGLWindow::onKeyEvent(Keyboard::Key key, std::int32_t /*scancode*/, Keyboard::Action action, std::uint8_t mods)
    {
        m_inputManager.onKeyboardEvent({key, action, mods});
    }

    void OpenGLWindow::onTextEnterEvent(std::uint32_t character) { m_inputManager.onTextEnterEvent(character); }

    void OpenGLWindow::onMouseScrollEvent(const glm::dvec2& delta) { m_inputManager.onMouseScrollEvent({delta}); }

    void OpenGLWindow::onMouseMove(const glm::dvec2& mousePosition)
    {
        m_inputManager.onMouseMoveEvent(Mouse::MoveEvent{mousePosition, mousePosition - m_inputManager.getMousePosition()});
    }

    void OpenGLWindow::onMouseButtonEvent(Mouse::Button button, Mouse::Action action, std::uint8_t mods)
    {
        m_inputManager.onMouseButtonEvent({button, action, m_inputManager.getMousePosition(), mods});
    }
} // namespace Turbo