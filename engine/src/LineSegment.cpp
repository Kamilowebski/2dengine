#include <engine/LineSegment.hpp>

#include <engine/PrimitiveRenderer.hpp>

namespace engine {

LineSegment::LineSegment(const Point2d& start, const Point2d& end)
    : m_start(start), m_end(end) {}

LineSegment::LineSegment(float x1, float y1, float x2, float y2)
    : m_start(x1, y1), m_end(x2, y2) {}

const Point2d& LineSegment::start() const { return m_start; }
const Point2d& LineSegment::end() const { return m_end; }

void LineSegment::setStart(const Point2d& start) { m_start = start; }
void LineSegment::setEnd(const Point2d& end) { m_end = end; }

void LineSegment::setEnds(const Point2d& start, const Point2d& end) {
    m_start = start;
    m_end = end;
}

void LineSegment::draw(PrimitiveRenderer& renderer, sf::Color color,
                       LineAlgorithm algorithm) const {
    // Wybór algorytmu robi renderer (jedno miejsce z switch)
    renderer.drawSegment(m_start, m_end, color, algorithm);
}

} // namespace engine
