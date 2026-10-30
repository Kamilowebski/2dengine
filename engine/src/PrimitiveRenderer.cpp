//
// Created by ksamiec1 on 30.10.2026.
//
#include <engine/PrimitiveRenderer.hpp>

#include <engine/Logger.hpp>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace engine {

namespace {

/// Zaokrągla współrzędną do najbliższego piksela.
int toPixel(double value) {
    return static_cast<int>(std::lround(value));
}

} // namespace

PrimitiveRenderer::PrimitiveRenderer(sf::Vector2u size)
    : m_sprite(m_texture) {
    if (size.x == 0 || size.y == 0) {
        throw std::invalid_argument("PrimitiveRenderer: rozmiar bufora musi być większy od zera");
    }
    resize(size);
}

void PrimitiveRenderer::resize(sf::Vector2u size) {
    if (size.x == 0 || size.y == 0) {
        return; // np. zminimalizowane okno, zostawiamy stary bufor
    }
    if (size == m_size) {
        return; // nic się nie zmieniło
    }

    m_size = size;
    // 4 bajty (R, G, B, A) na każdy piksel, na początku wszystko przezroczyste
    m_pixels.assign(static_cast<std::size_t>(size.x) * size.y * 4, 0);

    // Tekstura musi mieć ten sam rozmiar co bufor (update() wysyła cały bufor)
    if (!m_texture.resize(size)) {
        throw std::runtime_error("PrimitiveRenderer: nie można utworzyć tekstury");
    }
    // true = dopasuj prostokąt tekstury sprite'a do nowego rozmiaru
    m_sprite.setTexture(m_texture, true);

    LOG_DEBUG("Bufor programowy: " << size.x << "x" << size.y);
}

sf::Vector2u PrimitiveRenderer::size() const {
    return m_size;
}

void PrimitiveRenderer::clear(sf::Color color) {
    if (color == sf::Color::Transparent) {
        // Szybka ścieżka: same zera (tak najczęściej czyścimy)
        std::fill(m_pixels.begin(), m_pixels.end(), static_cast<std::uint8_t>(0));
        return;
    }
    for (std::size_t i = 0; i < m_pixels.size(); i += 4) {
        m_pixels[i + 0] = color.r;
        m_pixels[i + 1] = color.g;
        m_pixels[i + 2] = color.b;
        m_pixels[i + 3] = color.a;
    }
}

void PrimitiveRenderer::putPixel(int x, int y, sf::Color color) {
    // Przycinanie: piksel poza buforem pomijamy
    if (x < 0 || y < 0 || x >= static_cast<int>(m_size.x) || y >= static_cast<int>(m_size.y)) {
        return;
    }

    // Indeks pierwszego bajtu piksela: (wiersz * szerokość + kolumna) * 4
    const std::size_t index =
        (static_cast<std::size_t>(y) * m_size.x + static_cast<std::size_t>(x)) * 4;

    m_pixels[index + 0] = color.r;
    m_pixels[index + 1] = color.g;
    m_pixels[index + 2] = color.b;
    m_pixels[index + 3] = color.a;
}

void PrimitiveRenderer::drawLine(float x1f, float y1f, float x2f, float y2f, sf::Color color) {
    // Liczymy na double, żeby ograniczyć błędy zaokrągleń
    double x1 = x1f, y1 = y1f, x2 = x2f, y2 = y2f;

    const double dx = x2 - x1;
    const double dy = y2 - y1;

    // Punkt (początek = koniec): rysujemy jeden piksel
    if (std::abs(dx) < 1e-9 && std::abs(dy) < 1e-9) {
        putPixel(toPixel(x1), toPixel(y1), color);
        return;
    }

    if (std::abs(dx) >= std::abs(dy)) {
        // --- Linia mniej stroma: iterujemy po X, liczymy Y ---
        // Chcemy iść od mniejszego X do większego
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        const double slope = (y2 - y1) / (x2 - x1); // a: współczynnik kierunkowy
        const double offset = y1 - slope * x1;      // b: wyraz wolny, y = a*x + b

        for (int x = toPixel(x1); x <= toPixel(x2); ++x) {
            const double y = slope * x + offset;    // równanie prostej w każdym kroku
            putPixel(x, toPixel(y), color);
        }
    } else {
        // --- Linia stroma: iterujemy po Y, liczymy X ---
        // Zamieniamy role osi: x = a'*y + b' (unikamy dzielenia przez dx, które mogłoby być 0)
        if (y1 > y2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        const double slope = (x2 - x1) / (y2 - y1);
        const double offset = x1 - slope * y1;

        for (int y = toPixel(y1); y <= toPixel(y2); ++y) {
            const double x = slope * y + offset;
            putPixel(toPixel(x), y, color);
        }
    }
}

void PrimitiveRenderer::drawLineIncremental(float x1f, float y1f, float x2f, float y2f,
                                            sf::Color color) {
    const double x1 = x1f, y1 = y1f, x2 = x2f, y2 = y2f;

    const double dx = x2 - x1;
    const double dy = y2 - y1;

    // Liczba kroków = większa z różnic (tyle pikseli ma dłuższa oś)
    const long steps = std::lround(std::max(std::abs(dx), std::abs(dy)));

    // Punkt lub odcinek krótszy niż pół piksela: jeden piksel
    if (steps == 0) {
        putPixel(toPixel(x1), toPixel(y1), color);
        return;
    }

    // Przyrosty: o ile zmieniają się współrzędne w jednym kroku.
    // Jeden z nich ma wartość bezwzględną 1, drugi nie większą niż 1.
    const double xInc = dx / static_cast<double>(steps);
    const double yInc = dy / static_cast<double>(steps);

    double x = x1;
    double y = y1;

    // steps + 1 punktów: od początku do końca włącznie
    for (long i = 0; i <= steps; ++i) {
        putPixel(toPixel(x), toPixel(y), color);
        x += xInc; // tylko dodawanie, bez mnożenia
        y += yInc;
    }
}

void PrimitiveRenderer::drawLine(const Point2d& p1, const Point2d& p2, sf::Color color) {
    // Przeciążenie tylko przekazuje współrzędne punktów do wersji podstawowej
    drawLine(p1.x(), p1.y(), p2.x(), p2.y(), color);
}

void PrimitiveRenderer::drawLineIncremental(const Point2d& p1, const Point2d& p2, sf::Color color) {
    drawLineIncremental(p1.x(), p1.y(), p2.x(), p2.y(), color);
}

void PrimitiveRenderer::drawSegment(const Point2d& p1, const Point2d& p2, sf::Color color,
                                       LineAlgorithm algorithm) {
    switch (algorithm) {
        case LineAlgorithm::Basic:
            drawLine(p1, p2, color);
            break;
        case LineAlgorithm::Incremental:
            drawLineIncremental(p1, p2, color);
            break;
    }
}

void PrimitiveRenderer::drawPolyline(const std::vector<Point2d>& points, bool closed,
                                     sf::Color color, LineAlgorithm algorithm) {
    // Pusta lista: nie ma czego rysować
    if (points.empty()) {
        return;
    }

    // Jeden punkt: odcinek zerowej długości, czyli pojedynczy piksel
    if (points.size() == 1) {
        drawSegment(points.front(), points.front(), color, algorithm);
        return;
    }

    // Odcinki między kolejnymi punktami: (0,1), (1,2), ..., (n-2, n-1)
    for (std::size_t i = 0; i + 1 < points.size(); ++i) {
        drawSegment(points[i], points[i + 1], color, algorithm);
    }

    // Zamknięcie: ostatni punkt z pierwszym (przy 2 punktach to ten sam odcinek, więc pomijamy)
    if (closed && points.size() > 2) {
        drawSegment(points.back(), points.front(), color, algorithm);
    }
}

void PrimitiveRenderer::drawPolyline(const std::vector<LineSegment>& segments, bool closed,
                                         sf::Color color, LineAlgorithm algorithm) {
    // Każdy odcinek rysujemy tak, jak jest zapisany
    for (const LineSegment& segment : segments) {
        segment.draw(*this, color, algorithm);
    }

    // Zamknięcie: od końca ostatniego odcinka do początku pierwszego
    if (closed && segments.size() > 1) {
        drawSegment(segments.back().end(), segments.front().start(), color, algorithm);
    }
}

void PrimitiveRenderer::present(sf::RenderTarget& target) {
    // Wysyłamy cały bufor do tekstury (rozmiary są zgodne dzięki resize())
    m_texture.update(m_pixels.data());
    target.draw(m_sprite);
}

}