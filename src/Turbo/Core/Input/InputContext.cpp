#include "Turbo/Core/Input/InputContext.h"

namespace Turbo
{
    void InputContext::onKeyboardEvent(const Keyboard::Event& event) const
    {
        // Call every function bound to this event

        // Actions
        for (const auto& actionEvent : m_keyboardActionCallbacks[static_cast<std::uint16_t>(event.key)])
        {
            if (event.action == Keyboard::Action::Press && (actionEvent.modifiers & event.modifiers) == actionEvent.modifiers)
            {
                actionEvent.isDown = true;
                if (actionEvent.action == Keyboard::Action::Press)
                {
                    (*actionEvent.callable)();
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release && actionEvent.action == Keyboard::Action::Release && actionEvent.isDown == true)
            {
                actionEvent.isDown = false;
                (*actionEvent.callable)();
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
                    (*stateEvent.callable)(true);
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release)
            {
                if (stateEvent.isDown == true)
                {
                    stateEvent.isDown = false;
                    (*stateEvent.callable)(false);
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
                    (*rangeEvent.callable)(rangeEvent.direction == Direction::Negative ? -1 : 1);
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release)
            {
                if (rangeEvent.isDown == true)
                {
                    rangeEvent.isDown = false;
                    (*rangeEvent.callable)(0);
                }
            }
            
        }
    }
}