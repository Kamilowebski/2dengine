#include <engine/Application.hpp>
#include <engine/Logger.hpp>

#include <optional>
#include <algorithm>

namespace engine {
    Application::Application(const WindowConfig &config)
        : m_config(config), m_windowedResolution(config.resolution), m_fps(config.fontPath){
        LOG_INFO("Inicjalizacja aplikacji");
        m_fps.setVisible(config.showFps);
        createWindow();

        //DEMO
        m_demoCircle.setOrigin({m_demoCircle.getRadius(), m_demoCircle.getRadius()});
        m_demoCircle.setPosition(sf::Vector2f(m_config.resolution) / 2.f);
        m_demoCircle.setFillColor(sf::Color::White);
        LOG_INFO("Aplikacja zainicjalizowana");
    }

    Application::~Application() {
        LOG_INFO("Zamykanie aplikacji, zwalnianie zasobów");

        if (m_window.isOpen()) {
            m_window.close();
        }

        LOG_INFO("Okno zamknięte");
    }

    void Application::createWindow() {
        sf::VideoMode videoMode(m_config.resolution);
        const bool fullscreen = (m_config.mode == WindowMode::Fullscreen);

        if (fullscreen && !videoMode.isValid()) {
            LOG_WARNING(
                "Rozdzielczość "
                << m_config.resolution.x
                << "x"
                << m_config.resolution.y
                << " nieobsługiwana w pełnym ekranie, używam rozdzielczości pulpitu"
            );
            videoMode = sf::VideoMode::getDesktopMode();
            m_config.resolution = videoMode.size;
        }

        if (!fullscreen) {
            m_windowedResolution = m_config.resolution;
        }

        const sf::State state = fullscreen ? sf::State::Fullscreen : sf::State::Windowed;
        m_window.create(videoMode, m_config.title, state);
        const sf::ContextSettings settings =
        m_window.getSettings();

        LOG_INFO(
            "Okno utworzone: "
            << videoMode.size.x
            << "x"
            << videoMode.size.y
            << (fullscreen ? " (pełny ekran)" : " (okno)")
            << ", VSync: "
            << (m_config.vsync ? "włączony" : "wyłączony")
        );

        LOG_DEBUG(
            "OpenGL "
            << settings.majorVersion
            << "."
            << settings.minorVersion
            << ", antyaliasing: "
            << settings.antiAliasingLevel
        );

        if (m_config.vsync) {
            m_window.setVerticalSyncEnabled(true);
            m_window.setFramerateLimit(0);
        } else {
            m_window.setVerticalSyncEnabled(false);
            m_window.setFramerateLimit(m_config.framerateLimit);
        }

        m_input.reset(); // czyste wejścia
        LOG_INFO("Buforowanie okna: podwójne (rysowanie w tylnym buforze, zamiana w display())");
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

    const Input &Application::input() const {
        return m_input;
    }

    void Application::run() {
        LOG_INFO("Start głównej pętli");
        sf::Clock clock;

        while (m_window.isOpen()) {
            //Czas klatki  w sekundach
            const float dt = clock.restart().asSeconds();
            processEvents();
            update(dt);
            render();
        }
        LOG_INFO("Główna pętla zakończona");
    }

    void Application::processEvents() {
        m_input.beginFrame();
        while (const std::optional event = m_window.pollEvent()) {
            m_input.handleEvent(*event);
            if (event->is<sf::Event::Closed>()) {
                LOG_INFO("Żądanie zamknięcia okna (krzyżyk)");
                m_window.close();
            } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                m_window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(resized->size))));
            }
        }
    }

    void Application::update(float dt) {
        if (m_input.wasKeyPressed(sf::Keyboard::Key::F11)) {
            toggleFullscreen();
        }
        if (m_input.wasKeyPressed(sf::Keyboard::Key::F3)) {
            LOG_DEBUG(
                "Licznik FPS: "
                << (m_fps.isVisible()
                    ? "widoczny"
                    : "ukryty")
            );
            m_fps.toggleVisible();
            m_config.showFps = m_fps.isVisible();
        }
        if (m_input.wasKeyPressed(sf::Keyboard::Key::Escape)) {
            LOG_INFO("Zamknięcie klawiszem Escape");
            m_window.close();
        }

        //DEMO
        sf::Vector2f direction{0.f, 0.f};
        if (m_input.wasKeyPressed(sf::Keyboard::Key::W)) {
            direction.y -= 1.f;
        }
        if (m_input.wasKeyPressed(sf::Keyboard::Key::S)) {
            direction.y += 1.f;
        }
        if (m_input.wasKeyPressed(sf::Keyboard::Key::A)) {
            direction.x -= 1.f;
        }
        if (m_input.wasKeyPressed(sf::Keyboard::Key::D)) {
            direction.x += 1.f;
        }

        if (direction.lengthSquared() > 0.f) {
            m_demoCircle.move(direction.normalized() * kDemoSpeed * dt);
        }
        if (m_input.wasMouseButtonPressed(sf::Mouse::Button::Left)) {
            m_demoCircle.setPosition(sf::Vector2f(m_input.mousePosition()));
        }

        m_fps.update(dt);
    }

    void Application::render() {
        m_window.clear(sf::Color::Black);
        //RENDER
        m_fps.draw(m_window);
        m_window.draw(m_demoCircle);
        m_window.display();
    }
}
