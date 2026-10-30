#pragma once

#include <SFML/Window.hpp>

#include <array>
#include <cstddef>

/**
 * @file Input.hpp
 * @brief System wejścia wyjścia: klawiatura i mysz.
 */
namespace engine {
    /**
     * @brief Zbier zdarzenia wyjścia wejścia i udostępnia ich stan.
     * Jak używać:
     * 1. beginFrame() - na początku.
     * 2. handleEvent() dla każdego zdarzenia z okna.
     * 3. zapytania (isKeyDown, wasKeyPressed) w update
     */
    class Input {
    public:
        /**
         * @brief Czyści stany jednorazowe oraz przesunięcia.
         */
        void beginFrame();

        /**
         * @brief Przetwarza jedno zdarzenie z okna
         *
         * @param event
         */
        void handleEvent(const sf::Event& event);

        /**
         * @brief Zeruje wszystkie stany.
         */
        void reset();

        /**
         * @brief Czy klawisz jest aktualnie trzymany.
         *
         * @param key
         * @return
         */
        bool isKeyDown(sf::Keyboard::Key key) const;

        /**
         * @brief Czy klawisz zostal wcisniety w tej klatce.
         *
         * @param key
         * @return
         */
        bool wasKeyPressed(sf::Keyboard::Key key) const;

        /**
         * @brief Czy klawisz został puszczony w tej klatce.
         *
         * @param key
         * @return
         */
        bool wasKeyReleased(sf::Keyboard::Key key) const;

        /**
         * @brief Czy klawisz myszy sie aktualnie trzymany.
         *
         * @param button
         * @return
         */
        bool isMouseButtonDown(sf::Mouse::Button button) const;

        /**
         * @brief Czy przycisk myszy został puszczony w tej klatce
         *
         * @param button
         * @return
         */
        bool wasMouseButtonReleased(sf::Mouse::Button button) const;

        /**
         * @brief Czy przcisk myszy został kliknięty w tej klatce.
         *
         * @param button
         * @return
         */
        bool wasMouseButtonPressed(sf::Mouse::Button button) const;

        /**
         * @brief Pozycja kursora w tej klatce.
         *
         * @return
         */
        sf::Vector2i mousePosition() const;

        /**
         * @brief O ile się przesunął kursorw w tej klatce w px.
         *
         * @return
         */
        sf::Vector2i mouseDelta() const;

        /**
         * @brief Przewinięcie scrolla w tej klatce.
         * @return
         */
        float mouseWheelDelta() const;

    private:
        /**
         * @brief Stan pojedyńczego klawisza.
         */
        struct ButtonState {
            bool down = false; //Jest trzymany.
            bool pressed = false; // Wciśnięty w tej klatce.
            bool released = false; //Puszczony w tej klatce.
        };

        static constexpr std::size_t kKeyCount = sf::Keyboard::KeyCount;
        static constexpr std::size_t kButtonCount = sf::Mouse::ButtonCount;

        /**
         * @brief Sprawdz czy kod klawisza mieści się w tablicy.
         *
         * @param key
         * @return
         */
        static bool isValid(sf::Keyboard::Key key);

        /**
         * @brief Sprawdza czy klawisz myszy mieści się w tablicy.
         *
         * @param button
         * @return
         */
        static bool isValid(sf::Mouse::Button button);

        std::array<ButtonState, kKeyCount> m_keys{}; //Stany klawiszy
        std::array<ButtonState, kButtonCount> m_buttons{}; //Stany przycisków myszy
        sf::Vector2i m_mousePosition{0, 0}; // Pozycja kursora
        sf::Vector2i m_mouseDelta{0, 0}; // Przesunięcie w tej klatce
        float m_wheelDelta = 0.f; // Scroll w tej klatce
        bool m_hasMousePosition = false; // Czy znamy już pozycję
    };
}