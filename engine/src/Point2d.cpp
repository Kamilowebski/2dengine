#include <engine/Point2d.hpp>

#include <engine/PrimitiveRenderer.hpp>

#include <algorithm>
#include <cmath>

namespace engine {

    Point2d::Point2d(float x, float y, sf::Color color, int size)
        : m_x(x),
          m_y(y),
          m_color(color),
          m_size(std::max(1, size)) {}

    float Point2d::x() const { return m_x; }
    float Point2d::y() const { return m_y; }

    sf::Vector2f Point2d::position() const {
        return {m_x, m_y};
    }

    sf::Color Point2d::color() const {
        return m_color;
    }

    int Point2d::size() const {
        return m_size;
    }

    void Point2d::setX(float x) {
        m_x = x;
    }

    void Point2d::setY(float y) {
        m_y = y;
    }

    void Point2d::setPosition(float x, float y) {
        m_x = x;
        m_y = y;
    }

    void Point2d::setPosition(sf::Vector2f position) {
        m_x = position.x;
        m_y = position.y;
    }

    void Point2d::setColor(sf::Color color) {
        m_color = color;
    }

    void Point2d::setSize(int size) {
        m_size = std::max(1, size);
    }

    void Point2d::draw(PrimitiveRenderer& renderer) const {
        const int cx = static_cast<int>(std::lround(m_x));
        const int cy = static_cast<int>(std::lround(m_y));

        const int left = cx - m_size / 2;
        const int top = cy - m_size / 2;

        for (int dy = 0; dy < m_size; ++dy) {
            for (int dx = 0; dx < m_size; ++dx) {
                renderer.putPixel(left + dx,
                                  top + dy,
                                  m_color);
            }
        }
    }

} // namespace engine