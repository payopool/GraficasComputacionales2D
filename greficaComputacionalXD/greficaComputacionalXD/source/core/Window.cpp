#include "Core/Window.h"

Window::Window(int width, int height, const std::string& title)
{
    m_window = new sf::RenderWindow(sf::VideoMode({ static_cast<unsigned int>(width),
                                                    static_cast<unsigned int>(height) }),
        title, sf::Style::Default);

    if (m_window) {
        m_window->setFramerateLimit(60);
        MESSAGE("Window", "Window", "Window created successfully");
    }
    else
    {
        ERROR("Window", "Window", "Failed to create window");
    }
}

bool
Window::isOpen() const
{
    if (!m_window)
    {
        return m_window && m_window->isOpen();
    }
    else
    {
        ERROR("Window", "isOpen", "Widnow is null");
        return false;
    }

}

void Window::clear(const sf::Color& color)
{
    if (m_window)
    {
        m_window->clear(color);
    }
    else
    {
        ERROR("Window", "clear", "Window is null");
    }
}
void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& state)
{
    if (m_window)
    {
        m_window->draw(drawable, state);
    }
    else
    {
        ERROR("Window", "draw", "Window is null");
    }
}
void Window::display()
{
    if (m_window)
    {
        m_window->display();
    }
    else
    {
        ERROR("Window", "display", "Window is null");
    }
}
void Window::render() {
	deltatime = clock.restart();
}
void Window::destroy() {
    SAFE_PTR_RELEASE(m_window);
}

