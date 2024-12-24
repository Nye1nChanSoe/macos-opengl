#include "Logger.hpp"

Logger &Logger::Instance()
{
    static Logger instance;
    return instance;
}

// Private constructor
Logger::Logger()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    m_Logger = std::make_shared<spdlog::logger>("console_logger", console_sink);

    m_Logger->set_level(spdlog::level::trace);
    // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    m_Logger->set_pattern("%^[%r] [%l]%$ %v");

    spdlog::info("Logger initialized successfully!");
}

void Logger::TraceImpl(const std::string &message)
{
    m_Logger->trace(message);
}

void Logger::DebugImpl(const std::string &message)
{
    m_Logger->debug(message);
}

void Logger::InfoImpl(const std::string &message)
{
    m_Logger->info(message);
}

void Logger::WarnImpl(const std::string &message)
{
    m_Logger->warn(message);
}

void Logger::ErrorImpl(const std::string &message)
{
    m_Logger->error(message);
}

void Logger::CriticalImpl(const std::string &message)
{
    m_Logger->critical(message);
}
