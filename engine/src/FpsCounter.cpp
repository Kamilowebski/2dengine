#include "engine/FpsCounter.hpp"

#include <cmath>
#include <stdexcept>

namespace engine {
    FpsCounter::FpsCounter(const std::string& fontPath, unsigned int characterSize)
        : m_text(m_font, "FPS: 0", characterSize) {
        if (!m_font.openFromFile(fontPath)) {
            throw std::runtime_error("Nie można wczytać czcionki");
        }
        m_text.setFillColor(sf::Color::White);
        m_text.setOutlineColor(sf::Color::Black);
        m_text.setOutlineThickness(1.f);
        m_text.setPosition({8.f, 4.f});
    }

    void FpsCounter::update(float dt) {
        m_elapsed += dt;
        ++m_frames;

        if (m_elapsed >= kUpdateInterval) {
            m_fps = static_cast<float>(m_frames) / m_elapsed;
            m_text.setString("FPS: " + std::to_string(static_cast<int>(std::round(m_fps))));
            m_elapsed = 0.f;
            m_frames = 0;
        }
    }

    void FpsCounter::draw(sf::RenderTarget &target) {
        if (m_visible) {
            target.draw(m_text);
        }
    }

    void FpsCounter::setVisible(bool visible) {
        m_visible = visible;
    }

    void FpsCounter::toggleVisible() {
        m_visible = !m_visible;
    }

    bool FpsCounter::isVisible() const {
        return m_visible;
    }

    float FpsCounter::fps() const {
        return m_fps;
    }
}
