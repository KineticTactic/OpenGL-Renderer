#include "Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Log::s_clientLogger;

void Log::init() {
    // spdlog::set_pattern("%^[%T]: %v%$");
    spdlog::set_pattern("%v%$");

    // auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // console_sink->set_color_mode(spdlog::color_mode::always);

    s_clientLogger = spdlog::stdout_color_mt("App");
    s_clientLogger->set_level(spdlog::level::trace);
}