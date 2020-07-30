#include "Turbo/Core/Input/InputContext.h"

namespace Turbo
{
    bool InputContext::onKeyboardEvent(const Keyboard::Event& event) const
    {
        bool isEventHandled = false;

        // Call every function bound to this event
        for (const auto& callback : m_keyboardEventsCallbacks)
        {
            isEventHandled = isEventHandled || (*callback)(event);
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
} // namespace Turbo