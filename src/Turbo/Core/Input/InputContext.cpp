#include "Turbo/Core/Input/InputContext.h"

namespace Turbo
{
    InputContext::~InputContext() {}

    void InputContext::unbindOldHandles()
    {
        for (auto& handle : m_handlesToUnbind)
        {
            handle.removeFromVector();
        }
    }

    void InputContext::unbind(InputHandle* handle)
    {
        if (handle == nullptr)
        {
            return;
        }
        m_handlesToUnbind.push_back(*handle);
    }

    bool InputContext::onKeyboardEvent(const Keyboard::KeyEvent& event)
    {
        bool isEventHandled = false;

        // Call every function bound to this event
        if (event.action == Keyboard::Action::Press)
        {
            for (const auto& callback : m_keyPressCallbacks)
            {
                bool callbackHandledEvent = (*callback.second)(event);
                isEventHandled = isEventHandled || callbackHandledEvent;
            }
        }
        else if (event.action == Keyboard::Action::Release)
        {
            for (const auto& callback : m_keyReleaseCallbacks)
            {
                bool callbackHandledEvent = (*callback.second)(event);
                isEventHandled = isEventHandled || callbackHandledEvent;
            }
        }

        // Actions
        for (const auto& actionEvent : m_keyboardActionCallbacks[static_cast<std::uint16_t>(event.key)])
        {
            if (event.action == Keyboard::Action::Press && (actionEvent.second.modifiers & event.modifiers) == actionEvent.second.modifiers)
            {
                actionEvent.second.isDown = true;
                if (actionEvent.second.action == Keyboard::Action::Press)
                {
                    bool callbackHandledEvent = (*actionEvent.second.callable)();
                    isEventHandled = isEventHandled || callbackHandledEvent;
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release && actionEvent.second.action == Keyboard::Action::Release && actionEvent.second.isDown == true)
            {
                actionEvent.second.isDown = false;
                bool callbackHandledEvent = (*actionEvent.second.callable)();
                isEventHandled = isEventHandled || callbackHandledEvent;
            }
        }

        // States
        for (const auto& stateEvent : m_keyboardStateCallbacks[static_cast<std::uint16_t>(event.key)])
        {
            if (event.action == Keyboard::Action::Press)
            {
                if ((stateEvent.second.modifiers & event.modifiers) == stateEvent.second.modifiers)
                {
                    stateEvent.second.isDown = true;
                    bool callbackHandledEvent = (*stateEvent.second.callable)(true);
                    isEventHandled = isEventHandled || callbackHandledEvent;
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release)
            {
                if (stateEvent.second.isDown == true)
                {
                    stateEvent.second.isDown = false;
                    bool callbackHandledEvent = (*stateEvent.second.callable)(false);
                    isEventHandled = isEventHandled || callbackHandledEvent;
                }
            }
        }

        // Unidirectional Ranges
        for (const auto& rangeEvent : m_keyboardUnidirectionalRangeCallbacks[static_cast<std::uint16_t>(event.key)])
        {
            if (event.action == Keyboard::Action::Press)
            {
                if ((rangeEvent.second.modifiers & event.modifiers) == rangeEvent.second.modifiers)
                {
                    rangeEvent.second.isDown = true;
                    bool callbackHandledEvent = (*rangeEvent.second.callable)(rangeEvent.second.direction == Direction::Negative ? -1 : 1);
                    isEventHandled = isEventHandled || callbackHandledEvent;
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release)
            {
                if (rangeEvent.second.isDown == true)
                {
                    rangeEvent.second.isDown = false;
                    bool callbackHandledEvent = (*rangeEvent.second.callable)(0);
                    isEventHandled = isEventHandled || callbackHandledEvent;
                }
            }
        }

        return isEventHandled;
    }

    bool InputContext::onTextEnterEvent(std::uint32_t character)
    {
        bool isEventHandled = false;
        for (const auto& callback : m_textEnterCallbacks)
        {
            bool callbackHandledEvent = (*callback.second)(character);
            isEventHandled = isEventHandled || callbackHandledEvent;
        }

        return isEventHandled;
    }

    bool InputContext::onMouseMoveEvent(const Mouse::MoveEvent& event)
    {
        bool isEventHandled = false;
        for (const auto& callback : m_mouseMoveCallbacks)
        {
            bool callbackHandledEvent = (*callback.second)(event);
            isEventHandled = isEventHandled || callbackHandledEvent;
        }
        return isEventHandled;
    }

    bool InputContext::onMouseScrollEvent(const Mouse::ScrollEvent& event)
    {
        bool isEventHandled = false;
        for (const auto& callback : m_mouseScrollCallbacks)
        {
            bool callbackHandledEvent = (*callback.second)(event);
            isEventHandled = isEventHandled || callbackHandledEvent;
        }
        return isEventHandled;
    }

    bool InputContext::onMouseButtonEvent(const Mouse::ButtonEvent& event)
    {
        bool isEventHandled = false;
        if (event.action == Mouse::Action::Press)
        {
            for (const auto& callback : m_mousePressCallbacks)
            {
                bool callbackHandledEvent = (*callback.second)(event);
                isEventHandled = isEventHandled || callbackHandledEvent;
            }
        }
        else if (event.action == Mouse::Action::Release)
        {
            for (const auto& callback : m_mouseReleaseCallbacks)
            {
                bool callbackHandledEvent = (*callback.second)(event);
                isEventHandled = isEventHandled || callbackHandledEvent;
            }
        }
        return isEventHandled;
    }
} // namespace Turbo