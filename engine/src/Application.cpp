#include <engine/Application.hpp>
#include <optional>

namespace engine {
    Application::Application(const WindowConfig &config)
        : m_config(config), m_windowedResolution(config.resolution), m_fps(config.fontPath){
        m_fps.setVisible(config.showFps);
        createWindow();
    }

    void Application::createWindow() {
        sf::VideoMode videoMode(m_config.resolution);
        const bool fullscreen = (m_config.mode == WindowMode::Fullscreen);

        if (fullscreen && !videoMode.isValid()) {
            videoMode = sf::VideoMode::getDesktopMode();
            m_config.resolution = videoMode.size;
        }

        if (!fullscreen) {
            m_windowedResolution = m_config.resolution;
        }

        const sf::State state = fullscreen ? sf::State::Fullscreen : sf::State::Windowed;
        m_window.create(videoMode, m_config.title, state);

        if (m_config.vsync) {
            m_window.setVerticalSyncEnabled(true);
            m_window.setFramerateLimit(0);
        } else {
            m_window.setVerticalSyncEnabled(false);
            m_window.setFramerateLimit(m_config.framerateLimit);
        }
    }

    void Application::applyConfig(const WindowConfig &config) {
        m_config = config;
        m_fps.setVisible(config.showFps);
        createWindow();
    }

    void Application::setResolution(const sf::Vector2u &resolution) {
        WindowConfig newConfig = m_config;
        newConfig.resolution = resolution;
        applyConfig(newConfig);
    }

    void Application::setWindowMode(WindowMode mode) {
        WindowConfig newConfig = m_config;
        newConfig.mode = mode;
        if (mode == WindowMode::Fullscreen) {
            newConfig.resolution = sf::VideoMode::getDesktopMode().size;
        } else {
            newConfig.resolution = m_windowedResolution;
        }
        applyConfig(newConfig);
    }

    void Application::toggleFullscreen() {
        setWindowMode(m_config.mode == WindowMode::Windowed ? WindowMode::Fullscreen : WindowMode::Windowed);
    }

    const WindowConfig &Application::config() const {
        return m_config;
    }

    void Application::run() {
        sf::Clock clock;

        while (m_window.isOpen()) {
            //Czas klatki  w sekundach
            const float dt = clock.restart().asSeconds();
            processEvents();
            update(dt);
            render();
        }
    }

    void Application::processEvents() {
        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                m_window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(resized->size))));
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::F11) {
                    toggleFullscreen();
                } else if (key->code == sf::Keyboard::Key::F3) {}
                m_fps.toggleVisible();
            }
        }
    }

    void Application::update(float dt) {
        m_fps.update(dt);
    }

    void Application::render() {
        m_window.clear(sf::Color::Black);
        //RENDER
        m_fps.draw(m_window);
        m_window.display();
    }
}
