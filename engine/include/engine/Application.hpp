#pragma once

#include <engine/FpsCounter.hpp>

#include <SFML/Graphics.hpp>
#include <string>

/**
 * @file Application.hpp
 * @brief Główna klasa aplikacji silnika 2D.
 */
namespace engine {
    /**
     * @brief Tryb pracy okna.
     */
    enum class WindowMode {
        Windowed,
        Fullscreen
    };

    /**
     * @brief Konfiguracja okna aplikacji.
     */
    struct WindowConfig {
        sf::Vector2u resolution{1280, 720};
        WindowMode mode = WindowMode::Windowed;
        std::string title = "Engine2D";
        bool vsync = true;
        unsigned int framerateLimit = 60;
        bool showFps = true;
        std::string fontPath = "assets/fonts/DejaVuSans.ttf";
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

        /**
         * @brief Stosuje nową konfigurację.
         *
         * @param config Nowe ustawienia.
         */
        void applyConfig(const WindowConfig& config);

        /**
         * @brief Zmienia rozdzielczość
         *
         * @param resolution Nowa rozdzielczość w pixelach.
         */
        void setResolution(const sf::Vector2u& resolution);

        /**
         * @brief Zmienia tryb oknka
         *
         * @param mode Nowy tryb okna
         */
        void setWindowMode(WindowMode mode);

        /**
         * @brief Przełącza między fullscreenem a window mode.
         */
        void toggleFullscreen();

        /**
         *
         * @return Zwraca aktualną konfigurację okna.
         */
        [[nodiscard]] const WindowConfig& config() const;

    private:
        /**
         * @brief Tworzy okno na podstawie konfiguracji.
         */
        void createWindow();

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
        sf::Vector2u m_windowedResolution;
        WindowConfig m_config;
        FpsCounter m_fps;
    };
}