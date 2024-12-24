#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <mutex>

class Logger
{
private:
    std::shared_ptr<spdlog::logger> m_Logger;

public:
    // static wrappers
    static void Trace(const std::string &message) { Instance().TraceImpl(message); }
    static void Debug(const std::string &message) { Instance().DebugImpl(message); }
    static void Info(const std::string &message) { Instance().InfoImpl(message); }
    static void Warn(const std::string &message) { Instance().WarnImpl(message); }
    static void Error(const std::string &message) { Instance().ErrorImpl(message); }
    static void Critical(const std::string &message) { Instance().CriticalImpl(message); }

private:
    static Logger &Instance();

    // actual implementations
    void TraceImpl(const std::string &message);
    void DebugImpl(const std::string &message);
    void InfoImpl(const std::string &message);
    void WarnImpl(const std::string &message);
    void ErrorImpl(const std::string &message);
    void CriticalImpl(const std::string &message);

    Logger();

    // Delete copy and move constructors/assignment
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
};

#endif
