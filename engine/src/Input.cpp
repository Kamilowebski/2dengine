#include <engine/Input.hpp>

namespace engine {
    bool Input::isValid(sf::Keyboard::Key key) {
        const int index = static_cast<int>(key);
        return index >= 0 && static_cast<std::size_t>(index) < kKeyCount;
    }

    bool Input::isValid(sf::Mouse::Button button) {
        const int index = static_cast<int>(button);
        return index >= 0 && static_cast<std::size_t>(index) < kButtonCount;
    }

    void Input::beginFrame() {
        for (auto& s : m_keys) {
            s.pressed = false;
            s.released = false;
        }

        for (auto& s : m_buttons) {
            s.pressed = false;
            s.released = false;
        }
        m_mouseDelta = {0, 0};
        m_wheelDelta = 0.f;
    }

    void Input::handleEvent(const sf::Event &event) {
        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (isValid(key->code)) {
                // Ignorowanie powtorzen znaków przez trzymanie
                auto& s = m_keys[static_cast<std::size_t>(key->code)];
                if (!s.down) {
                    s.down = true;
                    s.pressed = true;
                }
            }
        } else if (const auto* key = event.getIf<sf::Event::KeyReleased>()) {
            if (isValid(key->code)) {
                auto& s = m_keys[static_cast<std::size_t>(key->code)];
                s.down = false;
                s.released = true;
            }
        } else if (const auto* btn = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (isValid(btn->button)) {
                auto& s = m_buttons[static_cast<std::size_t>(btn->button)];
                s.down = true;
                s.pressed = true;
            }
            m_mousePosition = btn->position;
            m_hasMousePosition = true;
        } else if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
            if (m_hasMousePosition) {
                m_mouseDelta += moved->position - m_mousePosition;
            }
            m_mousePosition = moved->position;
                m_hasMousePosition = true;
        } else if (const auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
            if (wheel->wheel == sf::Mouse::Wheel::Vertical) {
                m_wheelDelta += wheel->delta;
            }
        } else if (event.is<sf::Event::FocusLost>()) {
            reset();
        }
    }

    void Input::reset() {
        m_keys.fill({});
        m_buttons.fill({});
        m_mouseDelta = {0, 0};
        m_wheelDelta = 0.f;
    }

    bool Input::isKeyDown(sf::Keyboard::Key key) const {
        return isValid(key) && m_keys[static_cast<std::size_t>(key)].down;
    }

    bool Input::wasKeyPressed(sf::Keyboard::Key key) const {
        return isValid(key) && m_keys[static_cast<std::size_t>(key)].pressed;
    }

    bool Input::wasKeyReleased(sf::Keyboard::Key key) const {
        return isValid(key) && m_keys[static_cast<std::size_t>(key)].released;
    }

    bool Input::isMouseButtonDown(sf::Mouse::Button button) const {
        return isValid(button) && m_buttons[static_cast<std::size_t>(button)].down;
    }

    bool Input::wasMouseButtonPressed(sf::Mouse::Button button) const {
        return isValid(button) && m_buttons[static_cast<std::size_t>(button)].pressed;
    }

    bool Input::wasMouseButtonReleased(sf::Mouse::Button button) const {
        return isValid(button) && m_buttons[static_cast<std::size_t>(button)].released;
    }

    sf::Vector2i Input::mousePosition() const {
        return m_mousePosition;
    }

    sf::Vector2i Input::mouseDelta() const {
        return m_mouseDelta;
    }

    float Input::mouseWheelDelta() const {
        return m_wheelDelta;
    }
}
