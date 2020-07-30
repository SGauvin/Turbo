#ifndef INCLUDED_TURBO_INPUTCONTEXT_H
#define INCLUDED_TURBO_INPUTCONTEXT_H

#include <array>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "Turbo/Core/Callable.h"
#include "Turbo/Core/Input/Keyboard.h"
#include "Turbo/Core/Log.h"

namespace Turbo
{
    class InputContext
    {
    private:
        friend class InputManager;

    public:
        InputContext() = default;
        InputContext(const InputContext& other) = delete;
        InputContext(const InputContext&& other) = delete;

        enum class Direction : std::uint8_t
        {
            Positive = 0,
            Negative
        };

        void setIsEnabled(bool isEnabled) { m_isEnabled = isEnabled; }
        bool isEnabled() const { return m_isEnabled; }

        // -- Keyboard --
        template<typename F>
        void bindKeyToAction(F callback, Keyboard::Key key, Keyboard::Action action, std::uint8_t modifiers = Keyboard::Modifier::None);
        template<typename O, typename F>
        void bindKeyToAction(O* object, F callback, Keyboard::Key key, Keyboard::Action action,
                             std::uint8_t modifiers = Keyboard::Modifier::None);

        template<typename F>
        void bindKeyToState(F callback, Keyboard::Key key, std::uint8_t modifiers = Keyboard::Modifier::None);
        template<typename O, typename F>
        void bindKeyToState(O* object, F callback, Keyboard::Key key, std::uint8_t modifiers = Keyboard::Modifier::None);

        template<typename F>
        void bindKeyToRange(F callback, Keyboard::Key key, Direction direction, std::uint8_t modifiers = Keyboard::Modifier::None);
        template<typename O, typename F>
        void bindKeyToRange(O* object, F callback, Keyboard::Key key, Direction direction,
                            std::uint8_t modifiers = Keyboard::Modifier::None);

        template<typename F>
        void bindKeysToRange(F callback, Keyboard::Key positiveKey, Keyboard::Key negativeKey,
                             std::uint8_t modifiers = Keyboard::Modifier::None);
        template<typename O, typename F>
        void bindKeysToRange(O* object, F callback, Keyboard::Key positiveKey, Keyboard::Key negativeKey,
                             std::uint8_t modifiers = Keyboard::Modifier::None);
        // -- ~Keyboard --

        // template<typename F>
        // void bindMouseMovementToRange(F callback);
        // template<typename O, typename F>
        // void bindMouseMovementToRange(O* object, F callback);

    private:
        bool m_isEnabled = true;

        // -- Keyboard --
        bool onKeyboardEvent(const Keyboard::Event& event) const;

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

            KeyboardActionEvent(const KeyboardActionEvent&& other) = delete;
            const KeyboardActionEvent& operator=(const KeyboardActionEvent&& other) = delete;
            const KeyboardActionEvent& operator=(const KeyboardActionEvent& other) = delete;

            KeyboardActionEvent(const KeyboardActionEvent& other)
                : callable(other.callable->clone())
                , action(other.action)
                , modifiers(other.modifiers)
            {
            }
        };
        std::array<std::vector<KeyboardActionEvent>, static_cast<int>(Keyboard::Key::LastKey) + 1> m_keyboardActionCallbacks{};

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

            KeyboardStateEvent(const KeyboardStateEvent&& other) = delete;
            const KeyboardActionEvent& operator=(const KeyboardStateEvent&& other) = delete;
            const KeyboardActionEvent& operator=(const KeyboardStateEvent& other) = delete;

            KeyboardStateEvent(const KeyboardStateEvent& other)
                : callable(other.callable->clone())
                , modifiers(other.modifiers)
            {
            }
        };
        std::array<std::vector<KeyboardStateEvent>, static_cast<int>(Keyboard::Key::LastKey) + 1> m_keyboardStateCallbacks{};

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

            KeyboardUnidirectionalRangeEvent(const KeyboardUnidirectionalRangeEvent&& other) = delete;
            const KeyboardActionEvent& operator=(const KeyboardStateEvent&& other) = delete;
            const KeyboardActionEvent& operator=(const KeyboardStateEvent& other) = delete;

            KeyboardUnidirectionalRangeEvent(const KeyboardUnidirectionalRangeEvent& other)
                : callable(other.callable->clone())
                , direction(other.direction)
                , modifiers(other.modifiers)
            {
            }
        };
        std::array<std::vector<KeyboardUnidirectionalRangeEvent>, static_cast<int>(Keyboard::Key::LastKey) + 1>
            m_keyboardUnidirectionalRangeCallbacks{};

        // Bidirectional range
        struct KeyboardBidirectionalRangeEvent
        {
            std::unique_ptr<Turbo::Callable<bool, float>> callable;
            bool positiveKeyState = false;
            bool negativeKeyState = false;

            KeyboardBidirectionalRangeEvent(Turbo::Callable<bool, float>* callable)
                : callable(callable)
            {
            }

            KeyboardBidirectionalRangeEvent(const KeyboardBidirectionalRangeEvent&& other) = delete;
            const KeyboardActionEvent& operator=(const KeyboardStateEvent&& other) = delete;
            const KeyboardActionEvent& operator=(const KeyboardStateEvent& other) = delete;

            KeyboardBidirectionalRangeEvent(const KeyboardBidirectionalRangeEvent& other)
                : callable(other.callable->clone())
            {
            }

            bool onPositiveKeyStateChange(bool state)
            {
                positiveKeyState = state;
                return onKeyStateChange();
            }

            bool onNegativeKeyStateChange(bool state)
            {
                negativeKeyState = state;
                return onKeyStateChange();
            }

            bool onKeyStateChange()
            {
                float direction = positiveKeyState == negativeKeyState ? 0 : (positiveKeyState ? 1 : -1);
                return (*callable)(direction);
            }
        };
        std::vector<KeyboardBidirectionalRangeEvent> m_keyboardBidirectionalRangeCallbacks{};
        // -- ~Keyboard --
    };
} // namespace Turbo

#include "Turbo/Core/Input/InputContext.inl"

#endif // INCLUDED_TURBO_INPUTCONTEXT_H