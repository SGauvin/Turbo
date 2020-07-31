#include "Turbo/Core/Input/InputContext.h"

namespace Turbo
{
    InputContext::~InputContext() {}

    bool InputContext::onKeyboardEvent(const Keyboard::KeyEvent& event) const
    {
        bool isEventHandled = false;

        // Call every function bound to this event
        if (event.action == Keyboard::Action::Press)
        {
            for (const auto& callback : m_keyPressCallbacks)
            {
                isEventHandled = isEventHandled || (*callback)(event);
            }
        }
        else if (event.action == Keyboard::Action::Release)
        {
            for (const auto& callback : m_keyReleaseCallbacks)
            {
                isEventHandled = isEventHandled || (*callback)(event);
            }
        }

        // Actions
        for (const auto& actionEvent : m_keyboardActionCallbacks[static_cast<std::uint16_t>(event.key)])
        {
            if (event.action == Keyboard::Action::Press && (actionEvent.modifiers & event.modifiers) == actionEvent.modifiers)
            {
                actionEvent.isDown = true;
                if (actionEvent.action == Keyboard::Action::Press)
                {
                    isEventHandled = isEventHandled || (*actionEvent.callable)();
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release && actionEvent.action == Keyboard::Action::Release &&
                     actionEvent.isDown == true)
            {
                actionEvent.isDown = false;
                isEventHandled = isEventHandled || (*actionEvent.callable)();
            }
        }

        // States
        for (const auto& stateEvent : m_keyboardStateCallbacks[static_cast<std::uint16_t>(event.key)])
        {
            if (event.action == Keyboard::Action::Press)
            {
                if ((stateEvent.modifiers & event.modifiers) == stateEvent.modifiers)
                {
                    stateEvent.isDown = true;
                    isEventHandled = isEventHandled || (*stateEvent.callable)(true);
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release)
            {
                if (stateEvent.isDown == true)
                {
                    stateEvent.isDown = false;
                    isEventHandled = isEventHandled || (*stateEvent.callable)(false);
                }
            }
        }

        // Unidirectional Ranges
        for (const auto& rangeEvent : m_keyboardUnidirectionalRangeCallbacks[static_cast<std::uint16_t>(event.key)])
        {
            if (event.action == Keyboard::Action::Press)
            {
                if ((rangeEvent.modifiers & event.modifiers) == rangeEvent.modifiers)
                {
                    rangeEvent.isDown = true;
                    isEventHandled = isEventHandled || (*rangeEvent.callable)(rangeEvent.direction == Direction::Negative ? -1 : 1);
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release)
            {
                if (rangeEvent.isDown == true)
                {
                    rangeEvent.isDown = false;
                    isEventHandled = isEventHandled || (*rangeEvent.callable)(0);
                }
            }
        }

        return isEventHandled;
    }

    bool InputContext::onMouseMoveEvent(const Mouse::MoveEvent& event)
    {
        bool isEventHandled = false;
        for (const auto& callback : m_mouseMoveCallbacks)
        {
            isEventHandled = isEventHandled || (*callback)(event);
        }

        return isEventHandled;
    }

    bool InputContext::onMouseScrollEvent(const Mouse::ScrollEvent& event)
    {
        bool isEventHandled = false;
        for (const auto& callback : m_mouseScrollCallbacks)
        {
            isEventHandled = isEventHandled || (*callback)(event);
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
                isEventHandled = isEventHandled || (*callback)(event);
            }
        }
        else if (event.action == Mouse::Action::Release)
        {
            for (const auto& callback : m_mouseReleaseCallbacks)
            {
                isEventHandled = isEventHandled || (*callback)(event);
            }
        }

        return isEventHandled;
    }
} // namespace Turbo