#pragma once

#include <engine/FpsCounter.hpp>
#include <engine/Input.hpp>
#include <engine/PrimitiveRenderer.hpp>

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
     *
     *
     * @par Buforowanie
     * Okno SFML zawsze używa podwójnego buforowania: rysujemy w tylnym buforze,
     * a wywołanie sf::RenderWindow::display() zamienia go z przednim (widocznym).
     * SFML nie udostępnia parametru liczby buforów, a potrójne buforowanie zależy od
     * sterownika karty graficznej. Moment zamiany buforów ustawia WindowConfig::vsync.
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
         * @brief Zamyka aplikację i zwalnia zasoby
         */
        ~Application();
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

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

        /**
         * @brief Dostęp do systemu wejścia.
         *
         * @return
         */
        const Input& input() const;

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
        PrimitiveRenderer m_renderer;
        sf::Vector2u m_windowedResolution;
        WindowConfig m_config;
        FpsCounter m_fps;
        Input m_input;

        //TYCZMASOWE DANE:
        static constexpr float kDemoSpeed = 300.f;
        sf::CircleShape m_demoCircle{30.f};
        void drawPrimitivesDemo(); //DEMO
    };
}