#include <engine/Application.hpp>
#include <optional>

namespace engine {
    Application::Application(const WindowConfig &config)
        : m_window(sf::VideoMode({config.width, config.height}), config.title) {
        m_window.setFramerateLimit(config.framerateLimit);
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
            }
        }
    }

    void Application::update(float dt) {
        (void)dt;
    }

    void Application::render() {
        m_window.clear(sf::Color::Black);
        //RENDER
        m_window.display();
    }
}
