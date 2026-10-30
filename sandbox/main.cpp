#include <engine/Application.hpp>

#include <exception>
#include <iostream>

int main() {
    try {
        engine::WindowConfig config;
        config.resolution = {1280, 720};
        config.mode = engine::WindowMode::Windowed;
        config.title = "Silnik2D";
        config.vsync = false;

        engine::Application app(config);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Blad krytyczny: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}