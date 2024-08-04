#pragma once

#include "spdlog/spdlog.h"

class Log {
  public:
    static void init();

    inline static std::shared_ptr<spdlog::logger> &GetClientLogger() {
        return s_clientLogger;
    }

  private:
    static std::shared_ptr<spdlog::logger> s_clientLogger;
};

// Client logging macros
#define LOG_TRACE(...) ::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Log::GetClientLogger()->critical(__VA_ARGS__)