#pragma once

#include <cstdint>

namespace Turbo::Keyboard
{
    enum class Key : std::int16_t
    {
        Unknown = -1,
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        Num0 = 48,
        Num1 = 49,
        Num2 = 50,
        Num3 = 51,
        Num4 = 52,
        Num5 = 53,
        Num6 = 54,
        Num7 = 55,
        Num8 = 56,
        Num9 = 57,
        Semicolon = 59,
        Equal = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LBracket = 91,
        Backslash = 92,
        RBracket = 93,
        GraveAccent = 96,
        World1 = 161,
        World2 = 162,
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        Kp0 = 320,
        Kp1 = 321,
        Kp2 = 322,
        Kp3 = 323,
        Kp4 = 324,
        Kp5 = 325,
        Kp6 = 326,
        Kp7 = 327,
        Kp8 = 328,
        Kp9 = 329,
        KpDecimal = 330,
        KpDivide = 331,
        KpMultiply = 332,
        KpSubtract = 333,
        KpAdd = 334,
        KpEnter = 335,
        KpEqual = 336,
        LShift = 340,
        LControl = 341,
        LAlt = 342,
        LSuper = 343,
        RShift = 344,
        RControl = 345,
        RAlt = 346,
        RSuper = 347,
        Menu = 348,
        LastKey = Menu
    };

    enum class Action : std::uint8_t
    {
        Release = 0,
        Press = 1,
        Repeat = 2
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

    struct KeyEvent
    {
        Keyboard::Key key;
        Keyboard::Action action;
        std::uint8_t modifiers;
    };
} // namespace Turbo::Keyboard
