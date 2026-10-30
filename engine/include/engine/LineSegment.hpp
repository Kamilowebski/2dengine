#pragma once

#include <engine/LineAlgorithm.hpp>
#include <engine/Point2d.hpp>

#include <SFML/Graphics/Color.hpp>

/**
 * @file LineSegment.hpp
 * @brief Odcinek określony dwoma punktami.
 */

namespace engine {

class PrimitiveRenderer; // deklaracja z góry (pełna definicja tylko w .cpp)

/**
 * @brief Odcinek między dwoma punktami Point2d.
 *
 * Przechowuje początek i koniec. Przy rysowaniu używane są wyłącznie
 * współrzędne punktów. Kolor i rozmiar punktów nie mają znaczenia,
 * kolor odcinka podaje się w wywołaniu draw().
 */
class LineSegment {
public:
    /** @brief Tworzy odcinek zerowej długości w punkcie (0, 0). */
    LineSegment() = default;

    /**
     * @brief Tworzy odcinek z dwóch punktów.
     * @param start Początek odcinka.
     * @param end Koniec odcinka.
     */
    LineSegment(const Point2d& start, const Point2d& end);

    /**
     * @brief Tworzy odcinek ze współrzędnych końców.
     * @param x1 Początek, współrzędna X.
     * @param y1 Początek, współrzędna Y.
     * @param x2 Koniec, współrzędna X.
     * @param y2 Koniec, współrzędna Y.
     */
    LineSegment(float x1, float y1, float x2, float y2);

    /** @brief Zwraca początek odcinka. */
    [[nodiscard]] const Point2d& start() const;

    /** @brief Zwraca koniec odcinka. */
    [[nodiscard]] const Point2d& end() const;

    /**
     * @brief Ustawia początek odcinka.
     * @param start Nowy początek.
     */
    void setStart(const Point2d& start);

    /**
     * @brief Ustawia koniec odcinka.
     * @param end Nowy koniec.
     */
    void setEnd(const Point2d& end);

    /**
     * @brief Ustawia oba końce naraz.
     * @param start Nowy początek.
     * @param end Nowy koniec.
     */
    void setEnds(const Point2d& start, const Point2d& end);

    /**
     * @brief Rysuje odcinek w buforze programowym.
     * @param renderer Renderer, do którego bufora rysujemy.
     * @param color Kolor odcinka (domyślnie biały).
     * @param algorithm Algorytm rasteryzacji (domyślnie przyrostowy).
     */
    void draw(PrimitiveRenderer& renderer, sf::Color color = sf::Color::White,
              LineAlgorithm algorithm = LineAlgorithm::Incremental) const;

private:
    Point2d m_start; ///< Początek odcinka.
    Point2d m_end;   ///< Koniec odcinka.
};

} // namespace engine
