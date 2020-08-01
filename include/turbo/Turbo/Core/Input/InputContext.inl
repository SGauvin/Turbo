#include "Turbo/Core/Input/InputContext.h"

namespace Turbo
{
    template<typename F>
    InputHandle InputContext::bindKeyPressEvents(F callback)
    {
        m_keyPressCallbacks.emplace_back(std::pair(m_currentId, new Functor<bool, F, const Keyboard::KeyEvent&>(callback)));
        return InputHandle(m_keyPressCallbacks, this, m_currentId++);
    }

    template<typename O, typename F>
    InputHandle InputContext::bindKeyPressEvents(O* object, F callback)
    {
        m_keyPressCallbacks.emplace_back(std::pair(m_currentId, new Method<bool, O, F, const Keyboard::KeyEvent&>(object, callback)));
        return InputHandle(m_keyPressCallbacks, this, m_currentId++);
    }

    template<typename F>
    InputHandle InputContext::bindKeyReleaseEvents(F callback)
    {
        m_keyReleaseCallbacks.emplace_back(std::pair(m_currentId, new Functor<bool, F, const Keyboard::KeyEvent&>(callback)));
        return InputHandle(m_keyReleaseCallbacks, this, m_currentId++);
    }

    template<typename O, typename F>
    InputHandle InputContext::bindKeyReleaseEvents(O* object, F callback)
    {
        m_keyReleaseCallbacks.emplace_back(std::pair(m_currentId, new Method<bool, O, F, const Keyboard::KeyEvent&>(object, callback)));
        return InputHandle(m_keyReleaseCallbacks, this, m_currentId++);
    }

    template<typename F>
    InputHandle InputContext::bindTextEnterEvents(F callback)
    {
        m_textEnterCallbacks.emplace_back(std::pair(m_currentId, new Functor<bool, F, std::uint32_t>(callback)));
        return InputHandle(m_textEnterCallbacks, this, m_currentId++);
    }

    template<typename O, typename F>
    InputHandle InputContext::bindTextEnterEvents(O* object, F callback)
    {
        m_textEnterCallbacks.emplace_back(std::pair(m_currentId, new Method<bool, O, F, std::uint32_t>(object, callback)));
        return InputHandle(m_textEnterCallbacks, this, m_currentId++);
    }

    template<typename F>
    InputHandle InputContext::bindKeyToAction(F callback, Keyboard::Key key, Keyboard::Action action, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return InputHandle();
        }
        auto& vector = m_keyboardActionCallbacks[static_cast<std::uint16_t>(key)];
        vector.emplace_back(std::pair(m_currentId, KeyboardActionEvent(new Functor<bool, F>(callback), action, modifiers)));
        return InputHandle(vector, this, m_currentId++);
    }

    template<typename O, typename F>
    InputHandle InputContext::bindKeyToAction(O* object, F callback, Keyboard::Key key, Keyboard::Action action, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return InputHandle();
        }
        auto& vector = m_keyboardActionCallbacks[static_cast<std::uint16_t>(key)];
        vector.emplace_back(std::pair(m_currentId, KeyboardActionEvent(new Method<bool, O, F>(object, callback), action, modifiers)));
        return InputHandle(vector, this, m_currentId++);
    }

    template<typename F>
    InputHandle InputContext::bindKeyToState(F callback, Keyboard::Key key, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return InputHandle();
        }
        auto& vector = m_keyboardStateCallbacks[static_cast<std::uint16_t>(key)];
        vector.emplace_back(std::pair(m_currentId, KeyboardStateEvent(new Functor<bool, F, bool>(callback), modifiers)));
        return InputHandle(vector, this, m_currentId++);
    }

    template<typename O, typename F>
    InputHandle InputContext::bindKeyToState(O* object, F callback, Keyboard::Key key, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return InputHandle();
        }
        auto& vector = m_keyboardStateCallbacks[static_cast<std::uint16_t>(key)];
        vector.emplace_back(std::pair(m_currentId, KeyboardStateEvent(new Method<bool, O, F, bool>(object, callback), modifiers)));
        return InputHandle(vector, this, m_currentId++);
    }

    template<typename F>
    InputHandle InputContext::bindKeyToRange(F callback, Keyboard::Key key, Direction direction, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return InputHandle();
        }
        auto& vector = m_keyboardUnidirectionalRangeCallbacks[static_cast<std::uint16_t>(key)];
        vector.emplace_back(std::pair(m_currentId, KeyboardRangeEvent(new Functor<bool, F, float>(callback), direction, modifiers)));
        return InputHandle(vector, this, m_currentId++);
    }

    template<typename O, typename F>
    InputHandle InputContext::bindKeyToRange(O* object, F callback, Keyboard::Key key, Direction direction, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return InputHandle();
        }
        auto& vector = m_keyboardUnidirectionalRangeCallbacks[static_cast<std::uint16_t>(key)];
        vector.emplace_back(std::pair(m_currentId, KeyboardRangeEvent(new Method<bool, O, F, float>(object, callback), direction, modifiers)));
        return InputHandle(vector, this, m_currentId++);
    }

    template<typename F>
    InputHandle InputContext::bindMouseMoveEvents(F callback)
    {
        m_mouseMoveCallbacks.emplace_back(std::pair(m_currentId, new Functor<bool, F, const Mouse::MoveEvent&>(callback)));
        return InputHandle(m_mouseMoveCallbacks, this, m_currentId++);
    }

    template<typename O, typename F>
    InputHandle InputContext::bindMouseMoveEvents(O* object, F callback)
    {
        m_mouseMoveCallbacks.emplace_back(std::pair(m_currentId, new Method<bool, O, F, const Mouse::MoveEvent&>(object, callback)));
        return InputHandle(m_mouseMoveCallbacks, this, m_currentId++);
    }

    template<typename F>
    InputHandle InputContext::bindMouseScrollEvents(F callback)
    {
        m_mouseScrollCallbacks.emplace_back(std::pair(m_currentId, new Functor<bool, F, const Mouse::ScrollEvent&>(callback)));
        return InputHandle(m_mouseScrollCallbacks, this, m_currentId++);
    }

    template<typename O, typename F>
    InputHandle InputContext::bindMouseScrollEvents(O* object, F callback)
    {
        m_mouseScrollCallbacks.emplace_back(std::pair(m_currentId, new Method<bool, O, F, const Mouse::ScrollEvent&>(object, callback)));
        return InputHandle(m_mouseScrollCallbacks, this, m_currentId++);
    }

    template<typename F>
    InputHandle InputContext::bindMousePressEvents(F callback)
    {
        m_mousePressCallbacks.emplace_back(std::pair(m_currentId, new Functor<bool, F, const Mouse::ButtonEvent&>(callback)));
        return InputHandle(m_mousePressCallbacks, this, m_currentId++);
    }

    template<typename O, typename F>
    InputHandle InputContext::bindMousePressEvents(O* object, F callback)
    {
        m_mousePressCallbacks.emplace_back(std::pair(m_currentId, new Method<bool, O, F, const Mouse::ButtonEvent&>(object, callback)));
        return InputHandle(m_mousePressCallbacks, this, m_currentId++);
    }

    template<typename F>
    InputHandle InputContext::bindMouseReleaseEvents(F callback)
    {
        m_mouseReleaseCallbacks.emplace_back(std::pair(m_currentId, new Functor<bool, F, const Mouse::ButtonEvent&>(callback)));
        return InputHandle(m_mouseReleaseCallbacks, this, m_currentId++);
    }

    template<typename O, typename F>
    InputHandle InputContext::bindMouseReleaseEvents(O* object, F callback)
    {
        m_mouseReleaseCallbacks.emplace_back(std::pair(m_currentId, new Method<bool, O, F, const Mouse::ButtonEvent&>(object, callback)));
        return InputHandle(m_mouseReleaseCallbacks, this, m_currentId++);
    }
} // namespace Turbo