#include "Core/Window.h"

Window::Window(int width, int height, const std::string& title)
{
    m_window = new sf::RenderWindow(
        sf::VideoMode({ static_cast<unsigned int>(width),
                        static_cast<unsigned int>(height) }),
        title, sf::Style::Default);

    if (m_window) {
        m_window->setFramerateLimit(60);
        m_view = m_window->getDefaultView();  
        MESSAGE("Window", "Window", "Window created successfully");
    }
    else {
        ERROR("Window", "Window", "Failed to create window");
    }
}

Window::~Window() {
    SAFE_PTR_RELEASE(m_window);
}

bool Window::isOpen() const {
    if (m_window) {
        return m_window->isOpen();
    }
    else {
        ERROR("Window", "isOpen", "Window is null");
        return false;
    }
}

void Window::applyCameraView(const sf::Vector2f& position, float zoom) {
    if (!m_window) return;
    m_view.setCenter(position);
    m_view.setSize(m_window->getDefaultView().getSize() * (1.f / zoom));
    m_window->setView(m_view);
}


void Window::clear(const sf::Color& color) {
    if (m_window) {
        m_window->clear(color);
    }
    else {
        ERROR("Window", "clear", "Window is null");
    }
}

void Window::handleResize(const sf::Vector2u& size) {
    if (!m_window) {
        ERROR("Window", "handleResize", "Window is null");
        return;
    }

    sf::Vector2f fSize(static_cast<float>(size.x),
        static_cast<float>(size.y));

    m_view.setSize(fSize);
    m_view.setCenter(sf::Vector2f(fSize.x / 2.f, fSize.y / 2.f));
    m_window->setView(m_view);

    MESSAGE("Window", "handleResize", "Window resized successfully");
}

void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& state) {
    if (m_window) {
        m_window->draw(drawable, state);
    }
    else {
        ERROR("Window", "draw", "Window is null");
    }
}

void Window::display() {
    if (m_window) {
        m_window->display();
    }
    else {
        ERROR("Window", "display", "Window is null");
    }
}

void Window::render() {
    deltatime = clock.restart();
}

void Window::destroy() {
    SAFE_PTR_RELEASE(m_window);
}

void Window::close() {
    if (m_window) {
        m_window->close();
    }
    else {
        ERROR("Window", "close", "Window is null");
    }
}
