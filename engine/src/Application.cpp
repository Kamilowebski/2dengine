#include <engine/Application.hpp>
#include <engine/Logger.hpp>
#include <engine/Point2d.hpp>

#include <optional>
#include <algorithm>
#include <cmath>

namespace engine {
    Application::Application(const WindowConfig &config)
        : m_config(config), m_windowedResolution(config.resolution), m_fps(config.fontPath), m_renderer(config.resolution){
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
        m_renderer.resize(m_window.getSize());
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
                m_renderer.resize(resized->size);
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
        m_window.clear(sf::Color(30, 30, 40));
        m_window.draw(m_demoCircle);

        m_renderer.clear();
        drawPrimitivesDemo();
        m_renderer.present(m_window);
        //RENDER
        m_fps.draw(m_window);
        m_window.display();
    }

    void Application::drawPrimitivesDemo() {
        const sf::Vector2u size = m_renderer.size();
        const float cx = static_cast<float>(size.x) / 2.f;
        const float cy = static_cast<float>(size.y) / 2.f;
        constexpr float kPi = 3.14159265f;

        // Wachlarz 16 odcinków algorytmem przyrostowym: sprawdza wszystkie kierunki
        // (poziome, pionowe, skośne w każdej ćwiartce).
        for (int i = 0; i < 16; ++i) {
            const float angle = static_cast<float>(i) * 2.f * kPi / 16.f;
            m_renderer.drawLineIncremental(cx, cy,
                                           cx + std::cos(angle) * 200.f,
                                           cy + std::sin(angle) * 200.f,
                                           sf::Color(80, 220, 120));
        }

        // Odcinki do kursora myszy: oba algorytmy, różne kolory
        const float mx = static_cast<float>(m_input.mousePosition().x);
        const float my = static_cast<float>(m_input.mousePosition().y);

        // Punkty: środek ekranu, róg i kursor myszy
        Point2d center(cx, cy, sf::Color::White, 7);
        Point2d corner(20.f, 20.f, sf::Color(255, 200, 0), 7);
        Point2d mouse(static_cast<float>(m_input.mousePosition().x),
                      static_cast<float>(m_input.mousePosition().y),
                      sf::Color(255, 90, 90), 7);

        // Odcinki między punktami: oba algorytmy, różne kolory
        m_renderer.drawLine(corner, mouse, sf::Color(255, 200, 0));         // podstawowy: żółty
        m_renderer.drawLineIncremental(center, mouse, sf::Color(255, 90, 90)); // przyrostowy: czerwony

        // Rysowanie punktów (po liniach, żeby były na wierzchu)
        center.draw(m_renderer);
        corner.draw(m_renderer);
        mouse.draw(m_renderer);

        // Test setterów i getterów: punkt krąży wokół środka
        Point2d orbit;
        orbit.setColor(sf::Color(100, 180, 255));
        orbit.setSize(9);
        orbit.setPosition(cx + 120.f, cy);       // ustawienie pozycji
        orbit.setX(orbit.x() + 10.f);            // odczyt i zmiana współrzędnej
        orbit.draw(m_renderer);

    }
}
