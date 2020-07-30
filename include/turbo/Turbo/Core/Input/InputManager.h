#ifndef INCLUDED_TURBO_INPUTMANAGER_H
#define INCLUDED_TURBO_INPUTMANAGER_H

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "Turbo/Core/Input/InputContext.h"

namespace Turbo
{
    class State;

    class InputManager
    {
    private:
        friend class Window;
        friend class Application;

    public:
        InputManager() = default;
        InputManager(const InputManager& other) = delete;
        InputManager(const InputManager&& other) = delete;

        ~InputManager();

        InputContext* createInputContext();
        void removeInputContext(const InputContext* inputContext);

        // -- Keyboard --
        bool detectedKeyPressedEvent() const;
        bool detectedKeyReleasedEvent() const;
        bool isKeyDown(Keyboard::Key key) const;
        bool isKeyPressedEvent(Keyboard::Key key) const;
        bool isKeyReleasedEvent(Keyboard::Key key) const;
        // -- ~Keyboard --

        // -- Mouse --
        const glm::dvec2& getMousePosition() const;
        // -- ~Mouse --

    private:
        void resetTemporaryState();
        void onKeyboardEvent(const Keyboard::Event& event);
        void onMouseMove(const glm::dvec2& mousePosition);
        void onMouseButtonEvent(const Mouse::Event& event);

        void onStateChange(const State* state);

        std::unordered_map<const State*, std::vector<InputContext*>> m_inputContextListMap{};
        std::vector<InputContext*>* m_currentInputContextList = nullptr;

        // -- Keyboard --
        std::array<bool, static_cast<int>(Keyboard::Key::LastKey) + 1> m_keyboardHeldKeys{};
        std::array<bool, static_cast<int>(Keyboard::Key::LastKey) + 1> m_keyboardPressedKeys{};
        std::array<bool, static_cast<int>(Keyboard::Key::LastKey) + 1> m_keyboardReleasedKeys{};
        bool m_detectedKeyPressedEvent = false;
        bool m_detectedKeyReleasedEvent = false;
        // -- ~Keyboard --

        // -- Mouse --
        glm::dvec2 m_mousePosition = {0, 0};
        bool m_mouseMovedEvent = false;
        // -- ~Mouse --
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_INPUTMANAGER_H