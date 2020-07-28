#ifndef TURBO_LOG_H
#define TURBO_LOG_H

#include <memory>

#define TURBO_ENABLE_ENGINE_LOGS
#define TURBO_ENABLE_CLIENT_LOGS

#if defined TURBO_ENABLE_ENGINE_LOGS
#include "spdlog/spdlog.h"
#endif // TURBO_ENABLE_ENGINE_LOGS

namespace Turbo
{
    class Log
    {
    public:
        static void init();

        template<typename FormatString, typename... Args>
        inline static void engineInfo(const FormatString& fmt, const Args&... args)
        {
#if defined TURBO_ENABLE_ENGINE_LOGS
            s_engineLogger->info(fmt, args...);
#endif // TURBO_ENABLE_ENGINE_LOGS
        }

        template<typename FormatString, typename... Args>
        inline static void engineWarning(const FormatString& fmt, const Args&... args)
        {
#if defined TURBO_ENABLE_ENGINE_LOGS
            s_engineLogger->warn(fmt, args...);
#endif // TURBO_ENABLE_ENGINE_LOGS
        }

        template<typename FormatString, typename... Args>
        inline static void engineError(const FormatString& fmt, const Args&... args)
        {
#if defined TURBO_ENABLE_ENGINE_LOGS
            s_engineLogger->error(fmt, args...);
#endif // TURBO_ENABLE_ENGINE_LOGS
        }

        template<typename FormatString, typename... Args>
        inline static void info(const FormatString& fmt, const Args&... args)
        {
#if defined TURBO_ENABLE_CLIENT_LOGS
            s_clientLogger->info(fmt, args...);
#endif // TURBO_ENABLE_CLIENT_LOGS
        }

        template<typename FormatString, typename... Args>
        inline static void warning(const FormatString& fmt, const Args&... args)
        {
#if defined TURBO_ENABLE_CLIENT_LOGS
            s_clientLogger->warn(fmt, args...);
#endif // TURBO_ENABLE_CLIENT_LOGS
        }

        template<typename FormatString, typename... Args>
        inline static void error(const FormatString& fmt, const Args&... args)
        {
#if defined TURBO_ENABLE_CLIENT_LOGS
            s_clientLogger->error(fmt, args...);
#endif // TURBO_ENABLE_CLIENT_LOGS
        }

    private:
#if defined TURBO_ENABLE_CLIENT_LOGS
        static std::shared_ptr<spdlog::logger> s_engineLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
#endif // TURBO_ENABLE_CLIENT_LOGS
    };
} // namespace Turbo

#endif // TURBO_LOG_H:)