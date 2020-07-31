#include "Turbo/Core/Input/InputContext.h"

namespace Turbo
{
    template<typename F>
    void InputContext::bindKeyPressEvents(F callback)
    {
        m_keyPressCallbacks.emplace_back(new Functor<bool, F, const Keyboard::KeyEvent&>(callback));
    }

    template<typename O, typename F>
    void InputContext::bindKeyPressEvents(O* object, F callback)
    {
        m_keyPressCallbacks.emplace_back(new Method<bool, O, F, const Keyboard::KeyEvent&>(object, callback));
    }

    template<typename F>
    void InputContext::bindKeyReleaseEvents(F callback)
    {
        m_keyReleaseCallbacks.emplace_back(new Functor<bool, F, const Keyboard::KeyEvent&>(callback));
    }

    template<typename O, typename F>
    void InputContext::bindKeyReleaseEvents(O* object, F callback)
    {
        m_keyReleaseCallbacks.emplace_back(new Method<bool, O, F, const Keyboard::KeyEvent&>(object, callback));
    }

    template<typename F>
    void InputContext::bindKeyToAction(F callback, Keyboard::Key key, Keyboard::Action action, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardActionCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Functor<bool, F>(callback), action, modifiers);
    }

    template<typename O, typename F>
    void InputContext::bindKeyToAction(O* object, F callback, Keyboard::Key key, Keyboard::Action action, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardActionCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Method<bool, O, F>(object, callback),
                                                                                action,
                                                                                modifiers);
    }

    template<typename F>
    void InputContext::bindKeyToState(F callback, Keyboard::Key key, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        auto test = new (std::nothrow) Functor<bool, F, bool>(callback);
        m_keyboardStateCallbacks[static_cast<std::uint16_t>(key)].emplace_back(test, modifiers);
    }

    template<typename O, typename F>
    void InputContext::bindKeyToState(O* object, F callback, Keyboard::Key key, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardStateCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Method<bool, O, F, bool>(object, callback), modifiers);
    }

    template<typename F>
    void InputContext::bindKeyToRange(F callback, Keyboard::Key key, Direction direction, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardUnidirectionalRangeCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Functor<bool, F, float>(callback),
                                                                                             direction,
                                                                                             modifiers);
    }

    template<typename O, typename F>
    void InputContext::bindKeyToRange(O* object, F callback, Keyboard::Key key, Direction direction, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardUnidirectionalRangeCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Method<bool, O, F, float>(object,
                                                                                                                           callback),
                                                                                             direction,
                                                                                             modifiers);
    }

    template<typename F>
    void InputContext::bindKeysToRange(F callback, Keyboard::Key positiveKey, Keyboard::Key negativeKey, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(positiveKey) < 0 || positiveKey > Keyboard::Key::LastKey ||
            static_cast<std::int16_t>(negativeKey) < 0 || negativeKey > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardBidirectionalRangeCallbacks.emplace_back(new Functor<bool, F, float>(callback));
        bindKeyToState(&m_keyboardBidirectionalRangeCallbacks.back(),
                       &KeyboardBidirectionalRangeEvent::onPositiveKeyStateChange,
                       positiveKey,
                       modifiers);
        bindKeyToState(&m_keyboardBidirectionalRangeCallbacks.back(),
                       &KeyboardBidirectionalRangeEvent::onNegativeKeyStateChange,
                       negativeKey,
                       modifiers);
    }

    template<typename O, typename F>
    void InputContext::bindKeysToRange(O* object, F callback, Keyboard::Key positiveKey, Keyboard::Key negativeKey, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(positiveKey) < 0 || positiveKey > Keyboard::Key::LastKey ||
            static_cast<std::int16_t>(negativeKey) < 0 || negativeKey > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardBidirectionalRangeCallbacks.emplace_back(new Method<bool, O, F, float>(object, callback));
        bindKeyToState(&m_keyboardBidirectionalRangeCallbacks.back(),
                       &KeyboardBidirectionalRangeEvent::onPositiveKeyStateChange,
                       positiveKey,
                       modifiers);
        bindKeyToState(&m_keyboardBidirectionalRangeCallbacks.back(),
                       &KeyboardBidirectionalRangeEvent::onNegativeKeyStateChange,
                       negativeKey,
                       modifiers);
    }

    template<typename F>
    void InputContext::bindMouseMoveEvents(F callback)
    {
        m_mouseMoveCallbacks.emplace_back(new Functor<bool, F, const Mouse::MoveEvent&>(callback));
    }

    template<typename O, typename F>
    void InputContext::bindMouseMoveEvents(O* object, F callback)
    {
        m_mouseMoveCallbacks.emplace_back(new Method<bool, O, F, const Mouse::MoveEvent&>(object, callback));
    }

    template<typename F>
    void InputContext::bindMouseScrollEvents(F callback)
    {
        m_mouseScrollCallbacks.emplace_back(new Functor<bool, F, const Mouse::ScrollEvent&>(callback));
    }

    template<typename O, typename F>
    void InputContext::bindMouseScrollEvents(O* object, F callback)
    {
        m_mouseScrollCallbacks.emplace_back(new Method<bool, O, F, const Mouse::ScrollEvent&>(object, callback));
    }

    template<typename F>
    void InputContext::bindMousePressEvents(F callback)
    {
        m_mousePressCallbacks.emplace_back(new Functor<bool, F, const Mouse::ButtonEvent&>(callback));
    }

    template<typename O, typename F>
    void InputContext::bindMousePressEvents(O* object, F callback)
    {
        m_mousePressCallbacks.emplace_back(new Method<bool, O, F, const Mouse::ButtonEvent&>(object, callback));
    }

    template<typename F>
    void InputContext::bindMouseReleaseEvents(F callback)
    {
        m_mouseReleaseCallbacks.emplace_back(new Functor<bool, F, const Mouse::ButtonEvent&>(callback));
    }

    template<typename O, typename F>
    void InputContext::bindMouseReleaseEvents(O* object, F callback)
    {
        m_mouseReleaseCallbacks.emplace_back(new Method<bool, O, F, const Mouse::ButtonEvent&>(object, callback));
    }
} // namespace Turbo