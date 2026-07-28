#include "Core/Window.h"

Window::Window(int width, int height, const std::string& title)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = 4; // Levels: 0, 2, 4, 8, 16
    m_title = title;

    m_window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode({
            static_cast<unsigned int>(width),
            static_cast<unsigned int>(height)
            }),
        title,
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );

    if (m_window->isOpen()) {
        m_window->setVerticalSyncEnabled(true);
        handleResize(m_window->getSize());

        const sf::ContextSettings actualSettings = m_window->getSettings();
        MESSAGE("Window", "Window", "MSAA disponible: " + std::to_string(actualSettings.antiAliasingLevel) + "x");
        MESSAGE("Window", "Window", "Window created successfully");
    }
    else {
        ERROR("Window", "Window", "Failed to create window");
    }
}

bool Window::isOpen() const {
    return m_window && m_window->isOpen();
}

void Window::clear(const sf::Color& color) {
    if (m_window) m_window->clear(color);
}

void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
    if (m_window) m_window->draw(drawable, states);
}

void Window::display() {
    if (m_window) m_window->display();
}

void Window::close() {
    if (m_window) m_window->close();
}

void Window::handleResize(const sf::Vector2u& size) {
    if (!m_window) return;

    const sf::Vector2f fSize(static_cast<float>(size.x), static_cast<float>(size.y));
    m_baseViewSize = fSize;
    m_view.setSize(fSize);
    m_view.setCenter({ 0.f, 0.f });
    m_window->setView(m_view);
}

void Window::applyCameraView(const sf::Vector2f& center, float zoom, float rotationDeg) {
    if (!m_window) return;
    if (zoom <= 0.f) zoom = 1.f;

    m_view.setSize(m_baseViewSize / zoom);
    m_view.setCenter(center);
    m_view.setRotation(sf::degrees(rotationDeg));
    m_window->setView(m_view);
}

void Window::setMSAALevel(unsigned int level) {
    if (!m_window) return;

    const sf::Vector2u currentSize = m_window->getSize();
    const sf::Vector2i currentPosition = m_window->getPosition();
    const sf::View currentView = m_window->getView();

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = level;

    m_window->create(
        sf::VideoMode(currentSize),
        m_title,
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );

    m_window->setPosition(currentPosition);
    m_window->setView(currentView);
    m_window->setVerticalSyncEnabled(true);

    const sf::ContextSettings actualSettings = m_window->getSettings();
    MESSAGE("Window", "Window", "MSAA disponible: " + std::to_string(actualSettings.antiAliasingLevel) + "x");
}

void Window::update() {
    deltaTime = clock.restart();
}

void Window::render() {}

void Window::destroy() {
    m_window.reset();
}
