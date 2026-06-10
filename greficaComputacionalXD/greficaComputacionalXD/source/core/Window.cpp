#include "Core/Window.h"

/**
 * @brief Constructor de la clase Window.
 *
 * Crea una ventana SFML con las dimensiones y título especificados.
 *
 * @param width  Ancho de la ventana.
 * @param height Alto de la ventana.
 * @param title  Título de la ventana.
 */
Window::Window(int width, int height, const std::string& title)
{
    m_window = new sf::RenderWindow(
        sf::VideoMode({ static_cast<unsigned int>(width),
                        static_cast<unsigned int>(height) }),
        title, sf::Style::Default);

    if (m_window) {
        m_window->setFramerateLimit(60);
        MESSAGE("Window", "Window", "Window created successfully");
    }
    else {
        ERROR("Window", "Window", "Failed to create window");
    }
}

/**
 * @brief Destructor de la clase Window.
 *
 * Libera los recursos asociados a la ventana.
 */
Window::~Window() {
    SAFE_PTR_RELEASE(m_window);
}

/**
 * @brief Verifica si la ventana está abierta.
 *
 * @return true si la ventana existe y está abierta, false en caso contrario.
 */
bool Window::isOpen() const
{
    if (m_window) {
        return m_window && m_window->isOpen();
    }
    else {
        ERROR("Window", "isOpen", "Window is null");
        return false;
    }
}

/**
 * @brief Limpia la ventana con un color específico.
 *
 * @param color Color con el que se limpia la ventana.
 */
void Window::clear(const sf::Color& color)
{
    if (m_window) {
        m_window->clear(color);
    }
    else {
        ERROR("Window", "clear", "Window is null");
    }
}

/**
 * @brief Dibuja un objeto en la ventana.
 *
 * @param drawable Objeto SFML dibujable.
 * @param state    Estado de renderizado opcional.
 */
void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& state)
{
    if (m_window) {
        m_window->draw(drawable, state);
    }
    else {
        ERROR("Window", "draw", "Window is null");
    }
}

/**
 * @brief Muestra en pantalla lo que se ha dibujado.
 */
void Window::display()
{
    if (m_window) {
        m_window->display();
    }
    else {
        ERROR("Window", "display", "Window is null");
    }
}

/**
 * @brief Actualiza el tiempo delta para cálculos de frame.
 */
void Window::render() {
    deltatime = clock.restart();
}

/**
 * @brief Destruye la ventana liberando recursos.
 */
void Window::destroy() {
    SAFE_PTR_RELEASE(m_window);
}

/**
 * @brief Cierra la ventana.
 */
void Window::close() {
    if (m_window) {
        m_window->close();
    }
    else {
        ERROR("Window", "close", "Window is null");
    }
}

