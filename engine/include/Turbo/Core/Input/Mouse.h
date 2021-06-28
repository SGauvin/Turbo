#ifndef INCLUDED_TURBO_MOUSE_H
#define INCLUDED_TURBO_MOUSE_H

#include <cstdint>
#include <glm/glm.hpp>

namespace Turbo
{
    namespace Mouse
    {
        enum class Button : std::uint8_t
        {
            Left = 0,
            Right,
            Middle,
            Button4,
            Button5,
            Button6,
            Button7,
            Button8,
            LastButton = Button8
        };

        enum class Action : std::uint8_t
        {
            Release = 0,
            Press = 1,
        };

        namespace Modifier
        {
            static constexpr std::uint8_t None = 0x00;
            static constexpr std::uint8_t Shift = 0x01;
            static constexpr std::uint8_t Control = 0x02;
            static constexpr std::uint8_t Alt = 0x04;
            static constexpr std::uint8_t Super = 0x08;
            static constexpr std::uint8_t CapsLock = 0x10;
            static constexpr std::uint8_t NumLock = 0x2;
        } // namespace Modifier

        struct ButtonEvent
        {
            Mouse::Button button;
            Mouse::Action action;
            glm::dvec2 mousePosition;
            std::uint8_t modifiers;
        };

        struct MoveEvent
        {
            glm::dvec2 mousePosition;
            glm::dvec2 movement;
        };

        struct ScrollEvent
        {
            glm::dvec2 scrollDelta;
        };
    } // namespace Mouse
} // namespace Turbo

#endif // INCLUDED_TURBO_MOUSE_H