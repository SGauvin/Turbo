#ifndef INCLUDED_TURBO_WINDOW_H
#define INCLUDED_TURBO_WINDOW_H

#include <glm/glm.hpp>
#include "Turbo/Core/Input/InputManager.h"
#include "Turbo/Core/Renderer/RenderingApi.h"

class GLFWwindow;

namespace Turbo
{
    enum class WindowMode : std::uint8_t
    {
        Bordered = 0,
        FullScreen
    };

    struct WindowAttributes
    {
        std::string title = "";
        glm::uvec2 size = {300, 300};
        WindowMode mode = WindowMode::Bordered;
    };

    template<RenderingApi>
    class Window
    {
    public:
        Window(const WindowAttributes& windowAttributes, InputManager& inputManager);
        Window(const Window& other) = delete;
        Window(const Window&& other) = delete;
        Window& operator=(const Window& other) = delete;
        Window& operator=(const Window&& other) = delete;
        ~Window();

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
        GLFWwindow* getHandle() const;

        // Setters
        void setIsVSyncEnabled(bool isVsynEnabled);
        void setIsRawMouseEnabled(bool isRawMouseEnabled);
        void setIsResizable(bool isResizable);
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_WINDOW_H
