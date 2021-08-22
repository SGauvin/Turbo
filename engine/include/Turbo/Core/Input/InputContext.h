#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "Turbo/Core/Callback.h"
#include "Turbo/Core/Input/InputHandle.h"
#include "Turbo/Core/Input/Keyboard.h"
#include "Turbo/Core/Input/Mouse.h"
#include "Turbo/Core/Log.h"

namespace Turbo
{
    class InputContext
    {
    private:
        friend class InputManager;
        friend class InputHandle;

    public:
        InputContext() = default;
        InputContext(const InputContext& other) = delete;
        InputContext(const InputContext&& other) = delete;
        ~InputContext();

        enum class Direction : std::uint8_t
        {
            Positive = 0,
            Negative
        };

        bool isEnabled = true;

        // -- Keyboard --
        InputHandle bindKeyPressEvents(const Callable<bool, const Keyboard::KeyEvent&>& callback);
        InputHandle bindKeyReleaseEvents(const Callable<bool, const Keyboard::KeyEvent&>& callback);
        InputHandle bindTextEnterEvents(const Callable<bool, std::uint32_t>& callback);
        InputHandle bindKeyToAction(const Callable<bool>& callback, Keyboard::Key key, Keyboard::Action action,
                                    std::uint8_t modifiers = Keyboard::Modifier::None);
        InputHandle bindKeyToState(const Callable<bool, bool>& callback, Keyboard::Key key, std::uint8_t modifiers = Keyboard::Modifier::None);
        InputHandle bindKeyToRange(const Callable<bool, float>& callback, Keyboard::Key key, Direction direction,
                                   std::uint8_t modifiers = Keyboard::Modifier::None);
        InputHandle bindKeysToRange(const Callable<bool, float>& callback, Keyboard::Key positiveKey, Keyboard::Key negativeKey,
                                    std::uint8_t modifiers = Keyboard::Modifier::None);
        // -- ~Keyboard --

        // -- Mouse --
        InputHandle bindMouseMoveEvents(const Callable<bool, const Mouse::MoveEvent&>&);
        InputHandle bindMouseScrollEvents(const Callable<bool, const Mouse::ScrollEvent&>& callback);
        InputHandle bindMousePressEvents(const Callable<bool, const Mouse::ButtonEvent&>& callback);
        InputHandle bindMouseReleaseEvents(const Callable<bool, const Mouse::ButtonEvent&>& callback);
        // --~Mouse --

    private:
        // Current id counter
        std::uint32_t m_currentId = 0;
        std::vector<InputHandle> m_handlesToUnbind{};

        void unbindOldHandles();
        void unbind(InputHandle* handle);

        // -- Keyboard --
        bool onKeyboardEvent(const Keyboard::KeyEvent& event);
        bool onTextEnterEvent(std::uint32_t character);

        // All events
        std::vector<std::pair<std::uint32_t, Callable<bool, const Keyboard::KeyEvent&>>> m_keyPressCallbacks{};
        std::vector<std::pair<std::uint32_t, Callable<bool, const Keyboard::KeyEvent&>>> m_keyReleaseCallbacks{};
        std::vector<std::pair<std::uint32_t, Callable<bool, std::uint32_t>>> m_textEnterCallbacks{};

        // Action
        struct KeyboardActionEvent
        {
            Turbo::Callable<bool> callable;
            Keyboard::Action action;
            std::uint8_t modifiers;
            mutable bool isDown = false;

            KeyboardActionEvent(Turbo::Callable<bool> callable, Keyboard::Action action, std::uint8_t modifiers)
                : callable(callable)
                , action(action)
                , modifiers(modifiers)
            {
            }

            KeyboardActionEvent(const KeyboardActionEvent& other)
                : callable(other.callable)
                , action(other.action)
                , modifiers(other.modifiers)
            {
            }

            const KeyboardActionEvent& operator=(const KeyboardActionEvent& other)
            {
                callable = other.callable;
                action = other.action;
                modifiers = other.modifiers;
                return *this;
            }

            KeyboardActionEvent(const KeyboardActionEvent&& other) = delete;
            const KeyboardActionEvent& operator=(const KeyboardActionEvent&& other) = delete;
        };
        std::array<std::vector<std::pair<std::uint32_t, KeyboardActionEvent>>, static_cast<int>(Keyboard::Key::LastKey) + 1> m_keyboardActionCallbacks{};

        // State
        struct KeyboardStateEvent
        {
            Turbo::Callable<bool, bool> callable;
            std::uint8_t modifiers;
            mutable bool isDown = false;

            KeyboardStateEvent(Turbo::Callable<bool, bool> callable, std::uint8_t modifiers)
                : callable(callable)
                , modifiers(modifiers)
            {
            }

            KeyboardStateEvent(const KeyboardStateEvent& other)
                : callable(other.callable)
                , modifiers(other.modifiers)
            {
            }

            const KeyboardStateEvent& operator=(const KeyboardStateEvent& other)
            {
                callable = other.callable;
                modifiers = other.modifiers;
                return *this;
            }

            KeyboardStateEvent(const KeyboardStateEvent&& other) = delete;
            const KeyboardStateEvent& operator=(const KeyboardStateEvent&& other) = delete;
        };
        std::array<std::vector<std::pair<std::uint32_t, KeyboardStateEvent>>, static_cast<int>(Keyboard::Key::LastKey) + 1> m_keyboardStateCallbacks{};

        // Unidirectional range
        struct KeyboardUnidirectionalRangeEvent
        {
            Turbo::Callable<bool, float> callable;
            Direction direction;
            std::uint8_t modifiers;
            mutable bool isDown = false;

            KeyboardUnidirectionalRangeEvent(Turbo::Callable<bool, float> callable, Direction direction, std::uint8_t modifiers)
                : callable(callable)
                , direction(direction)
                , modifiers(modifiers)
            {
            }

            KeyboardUnidirectionalRangeEvent(const KeyboardUnidirectionalRangeEvent& other)
                : callable(other.callable)
                , direction(other.direction)
                , modifiers(other.modifiers)
            {
            }

            const KeyboardUnidirectionalRangeEvent& operator=(const KeyboardUnidirectionalRangeEvent& other)
            {
                callable = other.callable;
                direction = other.direction;
                modifiers = other.modifiers;
                return *this;
            }

            KeyboardUnidirectionalRangeEvent(const KeyboardUnidirectionalRangeEvent&& other) = delete;
            const KeyboardUnidirectionalRangeEvent& operator=(const KeyboardUnidirectionalRangeEvent&& other) = delete;
        };
        std::array<std::vector<std::pair<std::uint32_t, KeyboardUnidirectionalRangeEvent>>, static_cast<int>(Keyboard::Key::LastKey) + 1>
            m_keyboardUnidirectionalRangeCallbacks{};

        // -- ~Keyboard --

        // -- Mouse --

        bool onMouseMoveEvent(const Mouse::MoveEvent& event);
        bool onMouseScrollEvent(const Mouse::ScrollEvent& event);
        bool onMouseButtonEvent(const Mouse::ButtonEvent& event);

        // All events
        std::vector<std::pair<std::uint32_t, Callable<bool, const Mouse::MoveEvent&>>> m_mouseMoveCallbacks{};
        std::vector<std::pair<std::uint32_t, Callable<bool, const Mouse::ScrollEvent&>>> m_mouseScrollCallbacks{};
        std::vector<std::pair<std::uint32_t, Callable<bool, const Mouse::ButtonEvent&>>> m_mousePressCallbacks{};
        std::vector<std::pair<std::uint32_t, Callable<bool, const Mouse::ButtonEvent&>>> m_mouseReleaseCallbacks{};
    };
} // namespace Turbo
