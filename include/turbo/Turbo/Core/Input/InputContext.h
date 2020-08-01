#ifndef INCLUDED_TURBO_INPUTCONTEXT_H
#define INCLUDED_TURBO_INPUTCONTEXT_H

#include <array>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "Turbo/Core/Callable.h"
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

        template<typename F>
        InputHandle bindKeyPressEvents(F callback);
        template<typename O, typename F>
        InputHandle bindKeyPressEvents(O* object, F callback);

        template<typename F>
        InputHandle bindKeyReleaseEvents(F callback);
        template<typename O, typename F>
        InputHandle bindKeyReleaseEvents(O* object, F callback);

        template<typename F>
        InputHandle bindTextEnterEvents(F callback);
        template<typename O, typename F>
        InputHandle bindTextEnterEvents(O* object, F callback);

        template<typename F>
        InputHandle bindKeyToAction(F callback, Keyboard::Key key, Keyboard::Action action, std::uint8_t modifiers = Keyboard::Modifier::None);
        template<typename O, typename F>
        InputHandle bindKeyToAction(O* object, F callback, Keyboard::Key key, Keyboard::Action action, std::uint8_t modifiers = Keyboard::Modifier::None);

        template<typename F>
        InputHandle bindKeyToState(F callback, Keyboard::Key key, std::uint8_t modifiers = Keyboard::Modifier::None);
        template<typename O, typename F>
        InputHandle bindKeyToState(O* object, F callback, Keyboard::Key key, std::uint8_t modifiers = Keyboard::Modifier::None);

        template<typename F>
        InputHandle bindKeyToRange(F callback, Keyboard::Key key, Direction direction, std::uint8_t modifiers = Keyboard::Modifier::None);
        template<typename O, typename F>
        InputHandle bindKeyToRange(O* object, F callback, Keyboard::Key key, Direction direction, std::uint8_t modifiers = Keyboard::Modifier::None);

        template<typename F>
        InputHandle bindKeysToRange(F callback, Keyboard::Key positiveKey, Keyboard::Key negativeKey, std::uint8_t modifiers = Keyboard::Modifier::None);
        template<typename O, typename F>
        InputHandle bindKeysToRange(O* object, F callback, Keyboard::Key positiveKey, Keyboard::Key negativeKey,
                                    std::uint8_t modifiers = Keyboard::Modifier::None);

        // -- ~Keyboard --

        // -- Mouse --

        template<typename F>
        InputHandle bindMouseMoveEvents(F callback);
        template<typename O, typename F>
        InputHandle bindMouseMoveEvents(O* object, F callback);

        template<typename F>
        InputHandle bindMouseScrollEvents(F callback);
        template<typename O, typename F>
        InputHandle bindMouseScrollEvents(O* object, F callback);

        template<typename F>
        InputHandle bindMousePressEvents(F callback);
        template<typename O, typename F>
        InputHandle bindMousePressEvents(O* object, F callback);

        template<typename F>
        InputHandle bindMouseReleaseEvents(F callback);
        template<typename O, typename F>
        InputHandle bindMouseReleaseEvents(O* object, F callback);

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
        std::vector<std::pair<std::uint32_t, std::unique_ptr<Callable<bool, const Keyboard::KeyEvent&>>>> m_keyPressCallbacks{};
        std::vector<std::pair<std::uint32_t, std::unique_ptr<Callable<bool, const Keyboard::KeyEvent&>>>> m_keyReleaseCallbacks{};
        std::vector<std::pair<std::uint32_t, std::unique_ptr<Callable<bool, std::uint32_t>>>> m_textEnterCallbacks{};

        // Action
        struct KeyboardActionEvent
        {
            std::unique_ptr<Turbo::Callable<bool>> callable;
            Keyboard::Action action;
            std::uint8_t modifiers;
            mutable bool isDown = false;

            KeyboardActionEvent(Turbo::Callable<bool>* callable, Keyboard::Action action, std::uint8_t modifiers)
                : callable(callable)
                , action(action)
                , modifiers(modifiers)
            {
            }

            KeyboardActionEvent(const KeyboardActionEvent& other)
                : callable(other.callable->clone())
                , action(other.action)
                , modifiers(other.modifiers)
            {
            }

            const KeyboardActionEvent& operator=(const KeyboardActionEvent& other)
            {
                callable.reset(other.callable->clone());
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
            std::unique_ptr<Turbo::Callable<bool, bool>> callable;
            std::uint8_t modifiers;
            mutable bool isDown = false;

            KeyboardStateEvent(Turbo::Callable<bool, bool>* callable, std::uint8_t modifiers)
                : callable(callable)
                , modifiers(modifiers)
            {
            }

            KeyboardStateEvent(const KeyboardStateEvent& other)
                : callable(other.callable->clone())
                , modifiers(other.modifiers)
            {
            }

            const KeyboardStateEvent& operator=(const KeyboardStateEvent& other)
            {
                callable.reset(other.callable->clone());
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
            std::unique_ptr<Turbo::Callable<bool, float>> callable;
            Direction direction;
            std::uint8_t modifiers;
            mutable bool isDown = false;

            KeyboardUnidirectionalRangeEvent(Turbo::Callable<bool, float>* callable, Direction direction, std::uint8_t modifiers)
                : callable(callable)
                , direction(direction)
                , modifiers(modifiers)
            {
            }

            KeyboardUnidirectionalRangeEvent(const KeyboardUnidirectionalRangeEvent& other)
                : callable(other.callable->clone())
                , direction(other.direction)
                , modifiers(other.modifiers)
            {
            }

            const KeyboardUnidirectionalRangeEvent& operator=(const KeyboardUnidirectionalRangeEvent& other)
            {
                callable.reset(other.callable->clone());
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
        std::vector<std::pair<std::uint32_t, std::unique_ptr<Callable<bool, const Mouse::MoveEvent&>>>> m_mouseMoveCallbacks{};
        std::vector<std::pair<std::uint32_t, std::unique_ptr<Callable<bool, const Mouse::ScrollEvent&>>>> m_mouseScrollCallbacks{};
        std::vector<std::pair<std::uint32_t, std::unique_ptr<Callable<bool, const Mouse::ButtonEvent&>>>> m_mousePressCallbacks{};
        std::vector<std::pair<std::uint32_t, std::unique_ptr<Callable<bool, const Mouse::ButtonEvent&>>>> m_mouseReleaseCallbacks{};
    };
} // namespace Turbo

#include "Turbo/Core/Input/InputContext.inl"

#endif // INCLUDED_TURBO_INPUTCONTEXT_H