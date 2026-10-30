#include <engine/Application.hpp>
#include <engine/Logger.hpp>

#include <exception>
#include <iostream>
#include <cstdlib>

int main() {
    engine::LogConfig logConfig;
    logConfig.level = engine::LogLevel::Debug;

    engine::Logger::instance().init(logConfig);

    std::set_terminate([] {
        LOG_CRITICAL(
            "std::terminate: nieobsłużony wyjątek lub błąd krytyczny"
        );

        engine::Logger::instance().shutdown();

        std::abort();
    });

    LOG_INFO("=== Start sandbox ===");

    int exitCode = 0;

    try {
        engine::WindowConfig config;
        config.resolution = {1280, 720};
        config.mode = engine::WindowMode::Windowed;
        config.title = "Engine2D - sandbox";
        config.vsync = true;

        {
            engine::Application app(config);
            app.run();
        }
    }
    catch (const std::exception& e) {
        LOG_CRITICAL(
            "Nieobsłużony wyjątek: "
            << e.what()
        );

        exitCode = 1;
    }
    catch (...) {
        LOG_CRITICAL("Nieznany wyjątek");
        exitCode = 1;
    }

    LOG_INFO(
        "=== Koniec sandbox, kod wyjścia: "
        << exitCode
        << " ==="
    );

    engine::Logger::instance().shutdown();

    return exitCode;
}