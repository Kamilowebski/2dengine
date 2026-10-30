#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

/**
 * @file Point2d.hpp
 * @brief Punkt na płaszczyźnie 2D z kolorem i rozmiarem.
 */

namespace engine {

class PrimitiveRenderer;

/**
 * @brief Punkt 2D, który można odczytać, ustawić i narysować.
 *
 * Współrzędne są liczbami zmiennoprzecinkowymi w układzie bufora:
 * (0, 0) to lewy górny róg, X rośnie w prawo, Y w dół.
 * Przy rysowaniu współrzędne są zaokrąglane do pełnych pikseli.
 */
class Point2d {
public:
    /** @brief Tworzy biały punkt (0, 0) o rozmiarze 1 piksela. */
    Point2d() = default;

    /**
     * @brief Tworzy punkt.
     * @param x Współrzędna pozioma.
     * @param y Współrzędna pionowa.
     * @param color Kolor punktu (domyślnie biały).
     * @param size Bok rysowanego kwadratu w pikselach (minimum 1).
     */
    Point2d(float x,
            float y,
            sf::Color color = sf::Color::White,
            int size = 1);

    /** @brief Zwraca współrzędną poziomą. */
    [[nodiscard]] float x() const;

    /** @brief Zwraca współrzędną pionową. */
    [[nodiscard]] float y() const;

    /** @brief Zwraca pozycję jako wektor. */
    [[nodiscard]] sf::Vector2f position() const;

    /** @brief Zwraca kolor punktu. */
    [[nodiscard]] sf::Color color() const;

    /** @brief Zwraca rozmiar punktu (bok kwadratu w pikselach). */
    [[nodiscard]] int size() const;

    /** @brief Ustawia współrzędną poziomą. */
    void setX(float x);

    /** @brief Ustawia współrzędną pionową. */
    void setY(float y);

    /**
     * @brief Ustawia obie współrzędne naraz.
     * @param x Współrzędna pozioma.
     * @param y Współrzędna pionowa.
     */
    void setPosition(float x, float y);

    /**
     * @brief Ustawia pozycję z wektora.
     * @param position Nowa pozycja.
     */
    void setPosition(sf::Vector2f position);

    /** @brief Ustawia kolor punktu. */
    void setColor(sf::Color color);

    /**
     * @brief Ustawia rozmiar punktu.
     * @param size Bok kwadratu w pikselach; wartości mniejsze od 1 są zamieniane na 1.
     */
    void setSize(int size);

    /**
     * @brief Rysuje punkt w buforze programowym.
     *
     * Punkt jest kwadratem o boku size(), wyśrodkowanym na zaokrąglonych
     * współrzędnych. Części wychodzące poza bufor są pomijane.
     *
     * @param renderer Renderer, do którego bufora rysujemy.
     */
    void draw(PrimitiveRenderer& renderer) const;

private:
    float m_x = 0.f;
    float m_y = 0.f;
    sf::Color m_color = sf::Color::White;
    int m_size = 1;
};

} // namespace engine