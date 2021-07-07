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

    InputHandle InputContext::bindKeyPressEvents(const Callable<bool, const Keyboard::KeyEvent&>& callback)
    {
        m_keyPressCallbacks.emplace_back(std::pair(m_currentId, callback));
        return InputHandle(m_keyPressCallbacks, this, m_currentId++);
    }

    InputHandle InputContext::bindKeyReleaseEvents(const Callable<bool, const Keyboard::KeyEvent&>& callback)
    {
        m_keyReleaseCallbacks.emplace_back(std::pair(m_currentId, callback));
        return InputHandle(m_keyReleaseCallbacks, this, m_currentId++);
    }

    InputHandle InputContext::bindTextEnterEvents(const Callable<bool, std::uint32_t>& callback)
    {
        m_textEnterCallbacks.emplace_back(std::pair(m_currentId, callback));
        return InputHandle(m_textEnterCallbacks, this, m_currentId++);
    }

    InputHandle InputContext::bindKeyToAction(const Callable<bool>& callback, Keyboard::Key key, Keyboard::Action action, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return InputHandle();
        }
        auto& vector = m_keyboardActionCallbacks[static_cast<std::uint16_t>(key)];
        vector.emplace_back(std::pair(m_currentId, KeyboardActionEvent(callback, action, modifiers)));
        return InputHandle(vector, this, m_currentId++);
    }

    InputHandle InputContext::bindKeyToState(const Callable<bool, bool>& callback, Keyboard::Key key, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return InputHandle();
        }
        auto& vector = m_keyboardStateCallbacks[static_cast<std::uint16_t>(key)];
        vector.emplace_back(std::pair(m_currentId, KeyboardStateEvent(callback, modifiers)));
        return InputHandle(vector, this, m_currentId++);
    }

    InputHandle InputContext::bindKeyToRange(const Callable<bool, float>& callback, Keyboard::Key key, Direction direction, std::uint8_t modifiers)
    {
        return InputHandle();
        // if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        // {
        //     return InputHandle();
        // }
        // auto& vector = m_keyboardUnidirectionalRangeCallbacks[static_cast<std::uint16_t>(key)];
        // vector.emplace_back(std::pair(m_currentId, KeyboardRangeEvent(Callable<bool, float>(callback), direction, modifiers)));
        // return InputHandle(vector, this, m_currentId++);
    }

    InputHandle InputContext::bindMouseMoveEvents(const Callable<bool, const Mouse::MoveEvent&>& callback)
    {
        m_mouseMoveCallbacks.emplace_back(std::pair(m_currentId, callback));
        return InputHandle(m_mouseMoveCallbacks, this, m_currentId++);
    }

    InputHandle InputContext::bindMouseScrollEvents(const Callable<bool, const Mouse::ScrollEvent&>& callback)
    {
        m_mouseScrollCallbacks.emplace_back(std::pair(m_currentId, callback));
        return InputHandle(m_mouseScrollCallbacks, this, m_currentId++);
    }

    InputHandle InputContext::bindMousePressEvents(const Callable<bool, const Mouse::ButtonEvent&>& callback)
    {
        m_mousePressCallbacks.emplace_back(std::pair(m_currentId, callback));
        return InputHandle(m_mousePressCallbacks, this, m_currentId++);
    }

    InputHandle InputContext::bindMouseReleaseEvents(const Callable<bool, const Mouse::ButtonEvent&>& callback)
    {
        m_mouseReleaseCallbacks.emplace_back(std::pair(m_currentId, callback));
        return InputHandle(m_mouseReleaseCallbacks, this, m_currentId++);
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
                bool callbackHandledEvent = (callback.second)(event);
                isEventHandled = isEventHandled || callbackHandledEvent;
            }
        }
        else if (event.action == Keyboard::Action::Release)
        {
            for (const auto& callback : m_keyReleaseCallbacks)
            {
                bool callbackHandledEvent = (callback.second)(event);
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
                    bool callbackHandledEvent = (actionEvent.second.callable)();
                    isEventHandled = isEventHandled || callbackHandledEvent;
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release && actionEvent.second.action == Keyboard::Action::Release && actionEvent.second.isDown == true)
            {
                actionEvent.second.isDown = false;
                bool callbackHandledEvent = (actionEvent.second.callable)();
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
                    bool callbackHandledEvent = (stateEvent.second.callable)(true);
                    isEventHandled = isEventHandled || callbackHandledEvent;
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release)
            {
                if (stateEvent.second.isDown == true)
                {
                    stateEvent.second.isDown = false;
                    bool callbackHandledEvent = (stateEvent.second.callable)(false);
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
                    bool callbackHandledEvent = (rangeEvent.second.callable)(rangeEvent.second.direction == Direction::Negative ? -1.f : 1.f);
                    isEventHandled = isEventHandled || callbackHandledEvent;
                }
            }
            // Don't need modifiers for the release event : design choice
            else if (event.action == Keyboard::Action::Release)
            {
                if (rangeEvent.second.isDown == true)
                {
                    rangeEvent.second.isDown = false;
                    bool callbackHandledEvent = (rangeEvent.second.callable)(0);
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
            bool callbackHandledEvent = (callback.second)(character);
            isEventHandled = isEventHandled || callbackHandledEvent;
        }

        return isEventHandled;
    }

    bool InputContext::onMouseMoveEvent(const Mouse::MoveEvent& event)
    {
        bool isEventHandled = false;
        for (const auto& callback : m_mouseMoveCallbacks)
        {
            bool callbackHandledEvent = (callback.second)(event);
            isEventHandled = isEventHandled || callbackHandledEvent;
        }
        return isEventHandled;
    }

    bool InputContext::onMouseScrollEvent(const Mouse::ScrollEvent& event)
    {
        bool isEventHandled = false;
        for (const auto& callback : m_mouseScrollCallbacks)
        {
            bool callbackHandledEvent = (callback.second)(event);
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
                bool callbackHandledEvent = (callback.second)(event);
                isEventHandled = isEventHandled || callbackHandledEvent;
            }
        }
        else if (event.action == Mouse::Action::Release)
        {
            for (const auto& callback : m_mouseReleaseCallbacks)
            {
                bool callbackHandledEvent = (callback.second)(event);
                isEventHandled = isEventHandled || callbackHandledEvent;
            }
        }
        return isEventHandled;
    }
} // namespace Turbo