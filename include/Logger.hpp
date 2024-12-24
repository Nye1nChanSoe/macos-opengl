#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

class Logger
{
private:
    std::shared_ptr<spdlog::logger> m_Logger;

public:
    // static wrappers for logging with formatting support
    template <typename... Args>
    static void Trace(const std::string &format, Args &&...args)
    {
        Instance().TraceImpl(format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Debug(const std::string &format, Args &&...args)
    {
        Instance().DebugImpl(format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Info(const std::string &format, Args &&...args)
    {
        Instance().InfoImpl(format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Warn(const std::string &format, Args &&...args)
    {
        Instance().WarnImpl(format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Error(const std::string &format, Args &&...args)
    {
        Instance().ErrorImpl(format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Critical(const std::string &format, Args &&...args)
    {
        Instance().CriticalImpl(format, std::forward<Args>(args)...);
    }

private:
    // Singleton instance accessor
    static Logger &Instance();

    // Actual implementations for logging
    template <typename... Args>
    void TraceImpl(const std::string &format, Args &&...args)
    {
        m_Logger->trace(format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void DebugImpl(const std::string &format, Args &&...args)
    {
        m_Logger->debug(format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void InfoImpl(const std::string &format, Args &&...args)
    {
        m_Logger->info(format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void WarnImpl(const std::string &format, Args &&...args)
    {
        m_Logger->warn(format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void ErrorImpl(const std::string &format, Args &&...args)
    {
        m_Logger->error(format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void CriticalImpl(const std::string &format, Args &&...args)
    {
        m_Logger->critical(format, std::forward<Args>(args)...);
    }

    Logger();

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
};

#endif
