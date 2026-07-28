#include "Core/Window.h"

Window::Window(int width, int height, const std::string& title) {
    sf::VideoMode mode;
    mode.size = { static_cast<unsigned int>(width),
                 static_cast<unsigned int>(height) };

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = 4;

    m_window = new sf::RenderWindow();
    m_window->create(mode, sf::String(title), sf::Style::Default, settings);

    if (m_window->isOpen()) {
        m_window->setFramerateLimit(60);
        m_view = m_window->getDefaultView();
        MESSAGE("Window", "Window", "Window created successfully con MSAA 4x");
    }
    else {
        ERROR("Window", "Window", "Failed to create window");
    }
}


Window::~Window() {
    SAFE_PTR_RELEASE(m_window);
}

bool Window::isOpen() const {
    return m_window && m_window->isOpen();
}

void Window::applyCameraView(const sf::Vector2f& position, float zoom) {
    if (!m_window) return;
    m_view.setCenter(position);
    m_view.setSize(m_window->getDefaultView().getSize() * (1.f / zoom));
    m_window->setView(m_view);
}

void Window::clear(const sf::Color& color) {
    if (m_window) m_window->clear(color);
}

void Window::handleResize(const sf::Vector2u& size) {
    if (!m_window) return;

    sf::Vector2f fSize(static_cast<float>(size.x), static_cast<float>(size.y));
    m_view.setSize(fSize);
    m_view.setCenter(sf::Vector2f(fSize.x / 2.f, fSize.y / 2.f));
    m_window->setView(m_view);

    MESSAGE("Window", "handleResize", "Window resized successfully");
}

void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& state) {
    if (m_window) m_window->draw(drawable, state);
}

void Window::display() {
    if (m_window) m_window->display();
}

void Window::render() {
    deltatime = clock.restart();
}

void Window::destroy() {
    SAFE_PTR_RELEASE(m_window);
}

void Window::close() {
    if (m_window) m_window->close();
}
