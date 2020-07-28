// Include glad before Window.h
#include <glad/glad.h>
// ~ Glad

#include "Turbo/Core/Log.h"
#include "Turbo/Core/Window.h"

namespace Turbo
{
    Window::Window(const Attributes& windowAttributes, InputManager& inputManager)
        : m_window(glfwCreateWindow(windowAttributes.size.x, windowAttributes.size.y, windowAttributes.title.c_str(), nullptr, nullptr))
        , m_inputManager(inputManager)
    {
        if (m_window == nullptr)
        {
            Log::engineError("Error creating the window");
        }
        glfwMakeContextCurrent(m_window);

        std::int32_t gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (gladStatus == 0)
        {
            Log::engineError("Error loading openGL function with glad");
        }

        glViewport(0, 0, windowAttributes.size.x, windowAttributes.size.y);

        if (gladStatus != 0 && m_window != nullptr)
        {
            Log::engineInfo("Window created");
        }

        glfwSetWindowUserPointer(m_window, this);

        glfwSetKeyCallback(m_window,
                           [](GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods) {
                               static_cast<Window*>(glfwGetWindowUserPointer(window))
                                   ->onKeyAction(static_cast<Keyboard::Key>(key),
                                                 scancode,
                                                 static_cast<Keyboard::Action>(action),
                                                 static_cast<std::uint8_t>(mods));
                           });
    }

    Window::~Window()
    {
        glfwTerminate();
        Log::engineInfo("Window terminated");
    }

    void Window::processEvents()
    {
        m_inputManager.resetTemporaryState();
        glfwPollEvents();
    }

    void Window::init() { glfwInit(); }

    bool Window::isOpen() const
    {
        return !glfwWindowShouldClose(m_window);
    }

    void Window::onKeyAction(Keyboard::Key key, std::int32_t /*scancode*/, Keyboard::Action action, std::uint8_t mods)
    {
        m_inputManager.onKeyboardEvent({key, action, mods});
    }
} // namespace Turbo