#include "Turbo/Core/Input/InputContext.h"

namespace Turbo
{
    template<typename F>
    void InputContext::bindKeyToAction(F callback, Keyboard::Key key, Keyboard::Action action, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardActionCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Functor<F>(callback), action, modifiers);
    }

    template<typename O, typename F>
    void InputContext::bindKeyToAction(O* object, F callback, Keyboard::Key key, Keyboard::Action action, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardActionCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Method<O, F>(object, callback), action, modifiers);
    }

    template<typename F>
    void InputContext::bindKeyToState(F callback, Keyboard::Key key, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardStateCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Functor<F, bool>(callback), modifiers);
    }

    template<typename O, typename F>
    void InputContext::bindKeyToState(O* object, F callback, Keyboard::Key key, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardStateCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Method<O, F, bool>(object, callback), modifiers);
    }

    template<typename F>
    void InputContext::bindKeyToRange(F callback, Keyboard::Key key, Direction direction, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardUnidirectionalRangeCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Functor<F, float>(callback), direction, modifiers);
    }

    template<typename O, typename F>
    void InputContext::bindKeyToRange(O* object, F callback, Keyboard::Key key, Direction direction, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardUnidirectionalRangeCallbacks[static_cast<std::uint16_t>(key)].emplace_back(new Method<O, F, float>(object, callback), direction, modifiers);
    }

    template<typename F>
    void InputContext::bindKeysToRange(F callback, Keyboard::Key positiveKey, Keyboard::Key negativeKey, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(positiveKey) < 0 || positiveKey > Keyboard::Key::LastKey || static_cast<std::int16_t>(negativeKey) < 0 || negativeKey > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardBidirectionalRangeCallbacks.emplace_back(new Functor<F, float>(callback));
        bindKeyToState(&m_keyboardBidirectionalRangeCallbacks.back(), &KeyboardBidirectionalRangeEvent::onPositiveKeyStateChange, positiveKey, modifiers);
        bindKeyToState(&m_keyboardBidirectionalRangeCallbacks.back(), &KeyboardBidirectionalRangeEvent::onNegativeKeyStateChange, negativeKey, modifiers);
    }

    template<typename O, typename F>
    void InputContext::bindKeysToRange(O* object, F callback, Keyboard::Key positiveKey, Keyboard::Key negativeKey, std::uint8_t modifiers)
    {
        if (static_cast<std::int16_t>(positiveKey) < 0 || positiveKey > Keyboard::Key::LastKey || static_cast<std::int16_t>(negativeKey) < 0 || negativeKey > Keyboard::Key::LastKey)
        {
            return;
        }
        m_keyboardBidirectionalRangeCallbacks.emplace_back(new Method<O, F, float>(object, callback));
        bindKeyToState(&m_keyboardBidirectionalRangeCallbacks.back(), &KeyboardBidirectionalRangeEvent::onPositiveKeyStateChange, positiveKey, modifiers);
        bindKeyToState(&m_keyboardBidirectionalRangeCallbacks.back(), &KeyboardBidirectionalRangeEvent::onNegativeKeyStateChange, negativeKey, modifiers);
    }
}