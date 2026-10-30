#include <engine/Logger.hpp>

#include <SFML/System.hpp>

#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>

namespace engine {

namespace {

const char* levelName(LogLevel level) {
    switch (level) {
        case LogLevel::Trace:    return "TRACE";
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO ";
        case LogLevel::Warning:  return "WARN ";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRIT ";
        default:                 return "?????";
    }
}

std::string timestamp() {
    using namespace std::chrono;

    const auto now = system_clock::now();
    const std::time_t t = system_clock::to_time_t(now);

    const auto ms =
        duration_cast<milliseconds>(
            now.time_since_epoch()
        ) % 1000;

    std::tm tm{};

#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;

    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
        << '.'
        << std::setfill('0')
        << std::setw(3)
        << ms.count();

    return oss.str();
}

std::string baseName(const char* path) {
    const std::string p = path ? path : "";

    const auto pos = p.find_last_of("/\\");

    return pos == std::string::npos
           ? p
           : p.substr(pos + 1);
}

} // namespace

class Logger::SfmlErrBuffer : public std::streambuf {
protected:
    int_type overflow(int_type ch) override {
        if (traits_type::eq_int_type(ch, traits_type::eof())) {
            return traits_type::not_eof(ch);
        }

        if (ch == '\n') {
            flushLine();
        }
        else {
            m_line.push_back(static_cast<char>(ch));
        }

        return ch;
    }

private:
    void flushLine() {
        if (!m_line.empty()) {
            Logger::instance().log(
                LogLevel::Warning,
                "[SFML] " + m_line,
                "SFML"
            );

            m_line.clear();
        }
    }

    std::string m_line;
};

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

Logger::Logger() = default;

Logger::~Logger() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

void Logger::openFile(const std::string& filePath) {
    namespace fs = std::filesystem;

    std::error_code ec;

    const fs::path path(filePath);

    if (path.has_parent_path()) {
        fs::create_directories(
            path.parent_path(),
            ec
        );
    }

    if (fs::exists(path, ec)) {
        fs::path oldPath = path;

        oldPath.replace_extension(".old.log");

        fs::remove(oldPath, ec);
        fs::rename(path, oldPath, ec);
    }

    m_file.open(
        path,
        std::ios::out | std::ios::trunc
    );

    if (!m_file.is_open()) {
        std::cerr
            << "[Logger] Nie można otworzyć pliku logu: "
            << filePath
            << '\n';
    }
}

void Logger::init(const LogConfig& config) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_initialized) {
            return;
        }

        m_level = config.level;
        m_toConsole = config.toConsole;

        if (config.toFile) {
            openFile(config.filePath);
        }

        m_errBuffer =
            std::make_unique<SfmlErrBuffer>();

        m_oldErrBuf =
            sf::err().rdbuf(m_errBuffer.get());

        m_initialized = true;
    }

    log(
        LogLevel::Info,
        "Logger uruchomiony",
        "Logger.cpp"
    );
}

void Logger::shutdown() {
    log(
        LogLevel::Info,
        "Logger zamykany",
        "Logger.cpp"
    );

    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_initialized) {
        return;
    }

    if (m_oldErrBuf) {
        sf::err().rdbuf(m_oldErrBuf);
        m_oldErrBuf = nullptr;
    }

    m_errBuffer.reset();

    if (m_file.is_open()) {
        m_file.flush();
        m_file.close();
    }

    m_initialized = false;
}

void Logger::setLevel(LogLevel level) {
    m_level = level;
}

LogLevel Logger::level() const {
    return m_level;
}

bool Logger::isEnabled(LogLevel level) const {
    return level != LogLevel::Off &&
           level >= m_level.load();
}

void Logger::log(
    LogLevel level,
    const std::string& message,
    const char* file,
    int line
) {
    if (!isEnabled(level)) {
        return;
    }

    std::ostringstream oss;

    oss << '['
        << timestamp()
        << "] ["
        << levelName(level)
        << "] ";

    if (file && *file) {
        oss << '['
            << baseName(file);

        if (line > 0) {
            oss << ':'
                << line;
        }

        oss << "] ";
    }

    oss << message;

    const std::string text = oss.str();

    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_toConsole) {
        auto& out =
            (level >= LogLevel::Warning)
            ? std::cerr
            : std::cout;

        out << text << '\n';
    }

    if (m_file.is_open()) {
        m_file << text << '\n';
        m_file.flush();
    }
}

} // namespace engine