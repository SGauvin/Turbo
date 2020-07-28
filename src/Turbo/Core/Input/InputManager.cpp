#include "Turbo/Core/Input/InputManager.h"
#include <algorithm>

namespace Turbo
{
    void InputManager::resetTemporaryState()
    {
        std::fill(m_keyboardPressedKeys.begin(), m_keyboardPressedKeys.end(), false);
        std::fill(m_keyboardReleasedKeys.begin(), m_keyboardReleasedKeys.end(), false);
    }

    bool InputManager::isKeyDown(Keyboard::Key key) const
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return false;
        }
        return m_keyboardHeldKeys[static_cast<uint16_t>(key)];
    }

    bool InputManager::isKeyPressedEvent(Keyboard::Key key) const
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return false;
        }
        return m_keyboardPressedKeys[static_cast<uint16_t>(key)];
    }

    bool InputManager::isKeyReleasedEvent(Keyboard::Key key) const
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return false;
        }
        return m_keyboardReleasedKeys[static_cast<uint16_t>(key)];
    }

    void InputManager::onKeyboardEvent(const Keyboard::Event& event)
    {
        if (static_cast<std::int16_t>(event.key) < 0 || event.key > Keyboard::Key::LastKey)
        {
            return;
        }

        // Ignore key repeats for now
        if (event.action == Keyboard::Action::Repeat)
        {
            return;
        }

        if (event.action == Keyboard::Action::Press)
        {
            m_keyboardPressedKeys[static_cast<std::uint16_t>(event.key)] = true;
            m_keyboardHeldKeys[static_cast<std::uint16_t>(event.key)] = true;
        }
        else if (event.action == Keyboard::Action::Release)
        {
            m_keyboardReleasedKeys[static_cast<std::uint16_t>(event.key)] = true;
            m_keyboardHeldKeys[static_cast<std::uint16_t>(event.key)] = false;
        }

        for (const auto& inputContext : m_inputContexts)
        {
            if (inputContext.isEnabled())
            {
                inputContext.onKeyboardEvent(event);
            }
        }
    }
} // namespace Turbo