#ifndef TURBO_INPUTMANAGER_H
#define TURBO_INPUTMANAGER_H

#include <array>
#include <memory>
#include <vector>
#include "Turbo/Core/Callable.h"
#include "Turbo/Core/Input/InputContext.h"
#include "Turbo/Core/Input/Keyboard.h"

namespace Turbo
{
    class InputManager
    {
    private:
        friend class Window;

    public:
        InputContext& createInputContext()
        {
            m_inputContexts.push_back(InputContext{});
            return m_inputContexts.back();
        }

        bool isKeyDown(Keyboard::Key key) const;
        bool isKeyPressedEvent(Keyboard::Key key) const;
        bool isKeyReleasedEvent(Keyboard::Key key) const;

    private:
        void resetTemporaryState();
        void onKeyboardEvent(const Keyboard::Event& event);

        std::vector<InputContext> m_inputContexts{};

        // -- Keyboard --
        std::array<bool, static_cast<int>(Keyboard::Key::LastKey) + 1> m_keyboardHeldKeys{};
        std::array<bool, static_cast<int>(Keyboard::Key::LastKey) + 1> m_keyboardPressedKeys{};
        std::array<bool, static_cast<int>(Keyboard::Key::LastKey) + 1> m_keyboardReleasedKeys{};
        // -- ~Keyboard --
    };
} // namespace Turbo

#endif // TURBO_INPUTMANAGER_H