#include "Turbo/Core/Input/InputManager.h"
#include <algorithm>

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
            }
        }
    }

    bool InputManager::detectedKeyPressedEvent() const { return m_detectedKeyPressedEvent; }

    bool InputManager::detectedKeyReleasedEvent() const { return m_detectedKeyReleasedEvent; }

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

    const glm::dvec2& InputManager::getMousePosition() const { return m_mousePosition; }

    void InputManager::onPollEvents()
    {
        if (m_currentInputContextList != nullptr)
        {
            for (std::int32_t i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
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
            m_detectedKeyPressedEvent = true;
            m_keyboardPressedKeys[static_cast<std::uint16_t>(event.key)] = true;
            m_keyboardHeldKeys[static_cast<std::uint16_t>(event.key)] = true;
        }
        else if (event.action == Keyboard::Action::Release)
        {
            m_detectedKeyReleasedEvent = true;
            m_keyboardReleasedKeys[static_cast<std::uint16_t>(event.key)] = true;
            m_keyboardHeldKeys[static_cast<std::uint16_t>(event.key)] = false;
        }

        if (m_currentInputContextList != nullptr)
        {
            for (std::int32_t i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
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
        for (std::int32_t i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
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

        for (std::int32_t i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
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
        for (std::int32_t i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
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
            for (std::int32_t i = static_cast<std::int32_t>(m_currentInputContextList->size() - 1); i >= 0; --i)
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
