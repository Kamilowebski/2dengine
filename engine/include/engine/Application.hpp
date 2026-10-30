#pragma once

#include <SFML/Graphics.hpp>
#include <string>

/**
 * @file Application.hpp
 * @brief Główna klasa aplikacji silnika 2D.
 */
namespace engine {
    /**
     * @brief Konfiguracja okna aplikacji.
     */
    struct WindowConfig {
        unsigned int width = 1280;
        unsigned int height = 720;
        std::string title = "Engine";
        unsigned int framerateLimit = 60;
    };

    /**
     * @brief Główna pętla aplikacji, tworzy okno i obsługuje zdarzenia.
     */
    class Application {
    public:
        /**
         * @brief Tworzy aplikacji i otwiera okno.
         *
         * @param config Ustawienia okna.
         */
        explicit Application(const WindowConfig& config = {});

        /**
         * @brief Uruchamia główną pętlę gry.
         */
        void run();

    private:
        /**
         * @brief Odbiera i obsługuje zdarzenia z okna.
         */
        void processEvents();

        /**
         * @brief Aktualizuje stan aplikacji.
         *
         * @param dt Czas od poprzedniej klatki w sekundach.
         */
        void update(float dt);

        /**
         * @brief Rysuje bierzącą klatkę.
         */
        void render();

        /**
         * @brief Okno aplikacji.
         */
        sf::RenderWindow m_window;
    };
}