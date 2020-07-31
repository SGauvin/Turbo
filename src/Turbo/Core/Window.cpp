// Include glad before Window.h
#include <glad/glad.h>
// ~ Glad

#include "Turbo/Core/Log.h"
#include "Turbo/Core/Window.h"

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
    Window::Window(const Attributes& windowAttributes, InputManager& inputManager)
        : m_inputManager(inputManager)
    {
        create(windowAttributes);
        setIsRawMouseEnabled(m_isRawMouseEnabled);
    }

    Window::~Window()
    {
        destroy();
        glfwTerminate();
    }

    void Window::create(const Attributes& windowAttributes)
    {
        destroy();

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        if (windowAttributes.mode == Mode::Bordered)
        {
            m_window = glfwCreateWindow(windowAttributes.size.x, windowAttributes.size.y, windowAttributes.title.c_str(), nullptr, nullptr);
            m_size = {windowAttributes.size.x, windowAttributes.size.y};
        }
        else if (windowAttributes.mode == Mode::FullScreen)
        {
            m_window = glfwCreateWindow(windowAttributes.size.x, windowAttributes.size.y, windowAttributes.title.c_str(), monitor, nullptr);
            m_size = {windowAttributes.size.x, windowAttributes.size.y};
        }
        else if (windowAttributes.mode == Mode::Borderless)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            m_window = glfwCreateWindow(mode->width, mode->height, windowAttributes.title.c_str(), monitor, nullptr);
            m_size = {mode->width, mode->height};
        }

        m_mode = windowAttributes.mode;

        if (m_window == nullptr)
        {
            TURBO_ENGINE_ERROR("Error creating the window");
        }
        else
        {
            TURBO_ENGINE_INFO("Window created");
        }

        glfwMakeContextCurrent(m_window);

        std::int32_t gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (gladStatus == 0)
        {
            TURBO_ENGINE_ERROR("Error loading OpenGL functions with glad");
        }

        setIsVSyncEnabled(m_isVsyncEnabled);

        glViewport(0, 0, windowAttributes.size.x, windowAttributes.size.y);

        setCallbacks();

        setIsResizable(m_isResizable);

        glfwGetCursorPos(m_window, &m_inputManager.m_mousePosition.x, &m_inputManager.m_mousePosition.y);
    }

    void Window::destroy()
    {
        if (m_window == nullptr)
        {
            return;
        }
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        TURBO_ENGINE_INFO("Window terminated");
    }

    void Window::close() { m_shouldClose = true; }

    void Window::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    void Window::swapBuffers() { glfwSwapBuffers(m_window); }

    void Window::processEvents()
    {
        m_inputManager.resetTemporaryState();
        glfwPollEvents();
    }

    void Window::init() { glfwInit(); }

    bool Window::isOpen() const { return !glfwWindowShouldClose(m_window) && !m_shouldClose; }

    bool Window::isRawMouseAvailable() const { return glfwRawMouseMotionSupported(); }

    void Window::setIsVSyncEnabled(bool isVsyncEnabled)
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

    void Window::setIsRawMouseEnabled(bool isRawMouseEnabled)
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

    void Window::setIsResizable(bool isResizable)
    {
        m_isResizable = isResizable;

        if (!checkWindow(m_window))
            return;

        glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, isResizable);
    }

    void Window::setCallbacks()
    {
        glfwSetWindowUserPointer(m_window, this);

        // Window resize
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, std::int32_t width, std::int32_t height) {
            static_cast<Window*>(glfwGetWindowUserPointer(window))->onWindowResize({width, height});
        });

        // Keyboard
        glfwSetKeyCallback(m_window,
                           [](GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods) {
                               static_cast<Window*>(glfwGetWindowUserPointer(window))
                                   ->onKeyEvent(static_cast<Keyboard::Key>(key),
                                                scancode,
                                                static_cast<Keyboard::Action>(action),
                                                static_cast<std::uint8_t>(mods));
                           });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, std::uint32_t character) {
            static_cast<Window*>(glfwGetWindowUserPointer(window))->onTextEnterEvent(character);
        });

        // Cursor position
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
            static_cast<Window*>(glfwGetWindowUserPointer(window))->onMouseMove({xpos, ypos});
        });

        // Mouse buttons
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, std::int32_t button, std::int32_t action, std::int32_t mods) {
            static_cast<Window*>(glfwGetWindowUserPointer(window))
                ->onMouseButtonEvent(static_cast<Mouse::Button>(button),
                                     static_cast<Mouse::Action>(action),
                                     static_cast<std::uint8_t>(mods));
        });

        // Mouse scroll
        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double offsetX, double offsetY) {
            static_cast<Window*>(glfwGetWindowUserPointer(window))->onMouseScrollEvent({offsetX, offsetY});
        });
    }

    void Window::onWindowResize(glm::uvec2 windowSize) { m_size = windowSize; }

    void Window::onKeyEvent(Keyboard::Key key, std::int32_t /*scancode*/, Keyboard::Action action, std::uint8_t mods)
    {
        m_inputManager.onKeyboardEvent({key, action, mods});
    }

    void Window::onTextEnterEvent(std::uint32_t character) { m_inputManager.onTextEnterEvent(character); }

    void Window::onMouseScrollEvent(const glm::dvec2& delta) { m_inputManager.onMouseScrollEvent({delta}); }

    void Window::onMouseMove(const glm::dvec2& mousePosition)
    {
        m_inputManager.onMouseMoveEvent(Mouse::MoveEvent{mousePosition, mousePosition - m_inputManager.getMousePosition()});
    }

    void Window::onMouseButtonEvent(Mouse::Button button, Mouse::Action action, std::uint8_t mods)
    {
        m_inputManager.onMouseButtonEvent({button, action, m_inputManager.getMousePosition(), mods});
    }
} // namespace Turbo