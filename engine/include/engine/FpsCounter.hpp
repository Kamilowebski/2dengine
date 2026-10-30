#pragma once

#include <SFML/Graphics.hpp>
#include <string>

/**
 * @file FpsCounter.hpp
 * @brief Licznik klatek na sekundę rysowany w rogu ekranu.
 */
namespace engine {
    class FpsCounter {
    public:
        /**
         * @brief Tworzy licznik i wczytuje czcionkę.
         *
         * @param fontPath Ścieżka do pliku.
         * @param characterSize Rizmiar znaków w pikselach.
         *
         * @throw std::runtime_error gdy nie uda się wczytać czcionki.
         */
        explicit FpsCounter(const std::string& fontPath, unsigned int characterSize = 16);

        /**
         * @brief Zlicza klatkę i co jakiś czas przelicza FPS.
         *
         * @param dt Czas od poprzedniej klatki w sekundach.
         */
        void update(float dt);

        /**
         * @brief Rysuje licznik w lewym górnym rogu.
         *
         * @param target Cel rysowania
         */
        void draw(sf::RenderTarget& target);

        /**
         * @brief Ustawia widoczność licznika.
         *
         * @param visible
         */
        void setVisible(bool visible);

        /**
         * @brief Przełacza widoczność licznika.
         */
        void toggleVisible();

        /**
         * @brief Zwraca true jeśli licznik jest widoczny.
         *
         * @return
         */
        [[nodiscard]] bool isVisible() const;

        /**
         * @brief Zwraca ostatnio oblicznną wartość FPS.
         *
         * @return
         */
        [[nodiscard]] float fps() const;

    private:
        static constexpr float kUpdateInterval = 0.5f;

        sf::Font m_font; //Czcionka
        sf::Text m_text; //Napis do wyświetlenia
        float m_elapsed = 0.f; //Czas zebrany od ostatniego przeliczenia.
        unsigned int m_frames = 0; //Liczba klatek od ostatniego przeliczenia.
        float m_fps = 0; //Ostania obliczona wartość FPS.
        bool m_visible = true; // Czy rysować licznik.
    };
}