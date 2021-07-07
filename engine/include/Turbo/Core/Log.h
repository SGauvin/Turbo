#ifndef INCLUDED_TURBO_LOG_H
#define INCLUDED_TURBO_LOG_H

#include <assert.h>
#include <fmt/color.h>
#include <fmt/core.h>

// Client logs
#if defined TURBO_ENABLE_CLIENT_LOG
#define TURBO_INFO(...) \
    ::fmt::print(fg(fmt::color::light_slate_gray), "{}:{}: ", __FILE__, __LINE__); \
    ::fmt::print(fg(fmt::color::green), "Client info: "); \
    ::fmt::print(fg(fmt::color::light_green), __VA_ARGS__); \
    ::fmt::print("\n")
#define TURBO_WARNING(...) \
    ::fmt::print(fg(fmt::color::light_slate_gray), "{}:{}: ", __FILE__, __LINE__); \
    ::fmt::print(fg(fmt::color::yellow), "Client warning: "); \
    ::fmt::print(fg(static_cast<fmt::color>(0XFFFF73)), __VA_ARGS__); \
    ::fmt::print("\n")
#define TURBO_ERROR(...) \
    ::fmt::print(fg(fmt::color::light_slate_gray), "{}:{}: ", __FILE__, __LINE__); \
    ::fmt::print(fg(fmt::color::red), "Client error: "); \
    ::fmt::print(fg(fmt::color::indian_red), __VA_ARGS__); \
    ::fmt::print("\n")
#else
#define TURBO_INFO(...) ((void)(__VA_ARGS__))
#define TURBO_WARNING(...) ((void)(__VA_ARGS__))
#define TURBO_ERROR(...) ((void)(__VA_ARGS__))
#endif // TURBO_CLIENT_LOG

// Engine logs
#if defined TURBO_ENABLE_ENGINE_LOG

#define TURBO_ENGINE_INFO(...) \
    ::fmt::print(fg(fmt::color::light_slate_gray), "{}:{}: ", __FILE__, __LINE__); \
    ::fmt::print(fg(fmt::color::green), "Turbo info: "); \
    ::fmt::print(fg(fmt::color::light_green), __VA_ARGS__); \
    ::fmt::print("\n")
#define TURBO_ENGINE_WARNING(...) \
    ::fmt::print(fg(fmt::color::light_slate_gray), "{}:{}: ", __FILE__, __LINE__); \
    ::fmt::print(fg(fmt::color::yellow), "Turbo warning: "); \
    ::fmt::print(fg(static_cast<fmt::color>(0XFFFF73)), __VA_ARGS__); \
    ::fmt::print("\n")
#define TURBO_ENGINE_ERROR(...) \
    ::fmt::print(fg(fmt::color::light_slate_gray), "{}:{}: ", __FILE__, __LINE__); \
    ::fmt::print(fg(fmt::color::red), "Turbo error: "); \
    ::fmt::print(fg(fmt::color::indian_red), __VA_ARGS__); \
    ::fmt::print("\n")
#else
#define TURBO_ENGINE_INFO(...) ((void)(__VA_ARGS__))
#define TURBO_ENGINE_WARNING(...) ((void)(__VA_ARGS__))
#define TURBO_ENGINE_ERROR(...) ((void)(__VA_ARGS__))
#endif // TURBO_ENGINE_LOG

#if defined(TURBO_ENABLE_ASSERT) and !defined(NDEBUG)
#define TURBO_ASSERT(condition, message) \
    if (!condition) [[unlikely]] \
    { \
        TURBO_ENGINE_ERROR(message); \
    } \
    assert(condition)
#else
    #define TURBO_ASSERT(condition, message) (void)(condition); (void)(message)
#endif

#endif // INCLUDED_TURBO_LOG_H
