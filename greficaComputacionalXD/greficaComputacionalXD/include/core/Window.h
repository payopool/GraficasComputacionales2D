#pragma once
#include "Prerequisitos.h"

/**
 * @brief Clase que encapsula una ventana de renderizado usando SFML.
 */
class Window {
public:
    Window() = default;
    Window(int width, int height, const std::string& title);
    ~Window();

    bool isOpen() const;
    void clear(const sf::Color& color = sf::Color(0, 0, 255));
    void draw(const sf::Drawable& drawable,
        const sf::RenderStates& state = sf::RenderStates::Default);
    void display();
    void close();

    void handleResize(const sf::Vector2u& size);
    void applyCameraView(const sf::Vector2f& position, float zoom);

    void Update();
    void render();
    void destroy();


    /// Puntero a la ventana SFML.
    sf::RenderWindow* m_window = nullptr;

private:
    sf::View m_view;
    sf::Time deltatime;
    sf::Clock clock;
    std::string m_title;
};
