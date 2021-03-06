#include "Turbo/Core/Input/InputManager.h"
#include <algorithm>
#include <type_traits>

namespace Turbo
{
    InputManager::~InputManager()
    {
        for (const auto& it : m_inputContextListMap)
        {
            for (const auto& inputContext : it.second)
            {
                delete inputContext;
            }
        }
    }

    InputContext* InputManager::createInputContext()
    {
        if (m_currentInputContextList == nullptr)
        {
            TURBO_ENGINE_ERROR("Cannot create InputContext since there is no valid state.");
            return nullptr;
        }
        m_currentInputContextList->emplace_back(new InputContext());
        return m_currentInputContextList->back();
    }

    void InputManager::removeInputContext(const InputContext* inputContext)
    {
        if (m_currentInputContextList == nullptr)
        {
            TURBO_ENGINE_ERROR("Cannot remove InputContext since there is no valid state.");
            return;
        }

        for (auto it = m_currentInputContextList->begin(); it != m_currentInputContextList->end(); ++it)
        {
            if (inputContext == *it)
            {
                m_currentInputContextList->erase(it);
                break;
            }
        }
    }

    bool InputManager::detectedKeyPressedEvent() const
    {
        return m_detectedKeyPressedEvent;
    }

    bool InputManager::detectedKeyReleasedEvent() const
    {
        return m_detectedKeyReleasedEvent;
    }

    bool InputManager::isKeyDown(Keyboard::Key key) const
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return false;
        }

        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return m_keyboardHeldKeys[static_cast<uint16_t>(key)];
    }

    bool InputManager::isKeyPressedEvent(Keyboard::Key key) const
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return false;
        }

        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return m_keyboardPressedKeys[static_cast<uint16_t>(key)];
    }

    bool InputManager::isKeyReleasedEvent(Keyboard::Key key) const
    {
        if (static_cast<std::int16_t>(key) < 0 || key > Keyboard::Key::LastKey)
        {
            return false;
        }

        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return m_keyboardReleasedKeys[static_cast<uint16_t>(key)];
    }

    const glm::dvec2& InputManager::getMousePosition() const
    {
        return m_mousePosition;
    }

    void InputManager::onPollEvents()
    {
        if (m_currentInputContextList != nullptr)
        {
            for (auto i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
            {
                if ((*m_currentInputContextList)[i]->isEnabled)
                {
                    (*m_currentInputContextList)[i]->unbindOldHandles();
                }
            }
        }
        std::fill(m_keyboardPressedKeys.begin(), m_keyboardPressedKeys.end(), false);
        std::fill(m_keyboardReleasedKeys.begin(), m_keyboardReleasedKeys.end(), false);
        m_detectedKeyPressedEvent = false;
        m_detectedKeyReleasedEvent = false;
        m_mouseMovedEvent = false;
    }

    void InputManager::onKeyboardEvent(const Keyboard::KeyEvent& event)
    {
        using EnumType = std::underlying_type_t<decltype(event.key)>;
        if (static_cast<EnumType>(event.key) < 0 || event.key > Keyboard::Key::LastKey)
        {
            TURBO_ENGINE_WARNING(
                "Key {} is not valid. Should be between 0 and {}.", static_cast<EnumType>(event.key), static_cast<EnumType>(Keyboard::Key::LastKey));
            return;
        }

        // Ignore key repeats for now
        if (event.action == Keyboard::Action::Repeat)
        {
            return;
        }

        if (event.action == Keyboard::Action::Press)
        {
            m_detectedKeyPressedEvent = true;
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
            m_keyboardPressedKeys[static_cast<EnumType>(event.key)] = true;
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
            m_keyboardHeldKeys[static_cast<EnumType>(event.key)] = true;
        }
        else if (event.action == Keyboard::Action::Release)
        {
            m_detectedKeyReleasedEvent = true;
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
            m_keyboardReleasedKeys[static_cast<EnumType>(event.key)] = true;
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
            m_keyboardHeldKeys[static_cast<EnumType>(event.key)] = false;
        }

        if (m_currentInputContextList != nullptr)
        {
            for (auto i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
            {
                if ((*m_currentInputContextList)[i]->isEnabled)
                {
                    bool isEventHandled = (*m_currentInputContextList)[i]->onKeyboardEvent(event);
                    if (isEventHandled)
                    {
                        break;
                    }
                }
            }
        }
    }

    void InputManager::onTextEnterEvent(std::uint32_t character)
    {
        for (auto i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
        {
            if ((*m_currentInputContextList)[i]->isEnabled)
            {
                bool isEventHandled = (*m_currentInputContextList)[i]->onTextEnterEvent(character);
                if (isEventHandled)
                {
                    break;
                }
            }
        }
    }

    void InputManager::onMouseMoveEvent(const Mouse::MoveEvent& event)
    {
        m_mousePosition = event.mousePosition;
        m_mouseMovedEvent = true;

        for (auto i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
        {
            if ((*m_currentInputContextList)[i]->isEnabled)
            {
                bool isEventHandled = (*m_currentInputContextList)[i]->onMouseMoveEvent(event);
                if (isEventHandled)
                {
                    break;
                }
            }
        }
    }

    void InputManager::onMouseScrollEvent(const Mouse::ScrollEvent& event)
    {
        for (auto i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
        {
            if ((*m_currentInputContextList)[i]->isEnabled)
            {
                bool isEventHandled = (*m_currentInputContextList)[i]->onMouseScrollEvent(event);
                if (isEventHandled)
                {
                    break;
                }
            }
        }
    }

    void InputManager::onMouseButtonEvent(const Mouse::ButtonEvent& event)
    {
        if (m_currentInputContextList != nullptr)
        {
            for (auto i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
            {
                if ((*m_currentInputContextList)[i]->isEnabled)
                {
                    bool isEventHandled = (*m_currentInputContextList)[i]->onMouseButtonEvent(event);
                    if (isEventHandled)
                    {
                        break;
                    }
                }
            }
        }
    }

    void InputManager::onStateChange(const State* state)
    {
        if (state == nullptr)
        {
            m_currentInputContextList = nullptr;
            return;
        }

        auto it = m_inputContextListMap.find(state);
        if (it != m_inputContextListMap.cend())
        {
            m_currentInputContextList = &it->second;
        }
        else
        {
            m_inputContextListMap[state] = std::vector<InputContext*>();
            m_inputContextListMap[state].reserve(8);
            m_currentInputContextList = &m_inputContextListMap[state];
        }
    }

    void InputManager::onStateRemove(const State* state)
    {
        if (state == nullptr)
        {
            return;
        }

        auto it = m_inputContextListMap.find(state);
        if (it != m_inputContextListMap.cend())
        {
            auto vec = it->second;
            if (&vec == m_currentInputContextList)
            {
                m_currentInputContextList = nullptr;
            }

            for (auto inputContext : vec)
            {
                delete inputContext;
            }
            m_inputContextListMap.erase(it);
        }
    }

} // namespace Turbo
