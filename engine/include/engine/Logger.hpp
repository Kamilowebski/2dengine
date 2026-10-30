#pragma once

#include <atomic>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <streambuf>
#include <string>

namespace engine {

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical,
    Off
};

struct LogConfig {
    std::string filePath = "logs/engine.log";
    LogLevel level = LogLevel::Debug;
    bool toConsole = true;
    bool toFile = true;
};

class Logger {
public:
    /**
     * @brief Zwraca globalną instancję loggera.
     *
     * Logger tworzony jest przy pierwszym wywołaniu
     * zgodnie z wzorcem Meyer's Singleton.
     *
     * @return Referencja do loggera.
     */
    static Logger& instance();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void init(const LogConfig& config = {});
    void shutdown();

    void setLevel(LogLevel level);

    [[nodiscard]] LogLevel level() const;
    [[nodiscard]] bool isEnabled(LogLevel level) const;

    void log(
        LogLevel level,
        const std::string& message,
        const char* file = nullptr,
        int line = 0
    );

private:
    class SfmlErrBuffer;

    Logger();
    ~Logger();

    void openFile(const std::string& filePath);

    std::mutex m_mutex;
    std::ofstream m_file;
    std::atomic<LogLevel> m_level{LogLevel::Debug};

    bool m_toConsole = true;
    bool m_initialized = false;

    std::unique_ptr<SfmlErrBuffer> m_errBuffer;
    std::streambuf* m_oldErrBuf = nullptr;
};

} // namespace engine

#define ENGINE_LOG(level, ...)                                  \
    do {                                                        \
        if (::engine::Logger::instance().isEnabled(level)) {    \
            std::ostringstream engineLogStream_;                \
            engineLogStream_ << __VA_ARGS__;                    \
            ::engine::Logger::instance().log(                   \
                level,                                          \
                engineLogStream_.str(),                         \
                __FILE__,                                       \
                __LINE__                                        \
            );                                                  \
        }                                                       \
    } while (0)

#define LOG_TRACE(...) \
    ENGINE_LOG(::engine::LogLevel::Trace, __VA_ARGS__)

#define LOG_DEBUG(...) \
    ENGINE_LOG(::engine::LogLevel::Debug, __VA_ARGS__)

#define LOG_INFO(...) \
    ENGINE_LOG(::engine::LogLevel::Info, __VA_ARGS__)

#define LOG_WARNING(...) \
    ENGINE_LOG(::engine::LogLevel::Warning, __VA_ARGS__)

#define LOG_ERROR(...) \
    ENGINE_LOG(::engine::LogLevel::Error, __VA_ARGS__)

#define LOG_CRITICAL(...) \
    ENGINE_LOG(::engine::LogLevel::Critical, __VA_ARGS__)