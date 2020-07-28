#include "Turbo/Core/Log.h"

#if defined TURBO_ENABLE_ENGINE_LOGS
#include "spdlog/sinks/stdout_color_sinks.h"
#endif // TURBO_ENABLE_ENGINE_LOGS

namespace Turbo
{
#if defined TURBO_ENABLE_ENGINE_LOGS
    std::shared_ptr<spdlog::logger> Log::s_engineLogger;
    std::shared_ptr<spdlog::logger> Log::s_clientLogger;
#endif // TURBO_ENABLE_ENGINE_LOGS

    void Log::init()
    {
#if defined TURBO_ENABLE_ENGINE_LOGS
        spdlog::set_pattern("[%T] %n: %^%v%$");

        s_engineLogger = spdlog::stdout_color_mt("Turbo");
        s_clientLogger = spdlog::stdout_color_mt("Client");
#endif // TURBO_ENABLE_ENGINE_LOGS
    }
} // namespace Turbo
