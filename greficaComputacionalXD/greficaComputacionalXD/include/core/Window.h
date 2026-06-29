#pragma once
#include "Prerequisitos.h"

/**
 * @brief Clase que encapsula una ventana de renderizado usando SFML.
 *
 * Proporciona métodos para manejar la ventana, dibujar elementos,
 * actualizar estados y controlar el ciclo de vida de la aplicación.
 */
class Window {
public:
    /**
     * @brief Constructor por defecto.
     */
    Window() = default;

    /**
     * @brief Constructor que inicializa la ventana con dimensiones y título.
     * @param width  Ancho de la ventana.
     * @param hight  Alto de la ventana (typo: debería ser height).
     * @param title  Título de la ventana.
     */
    Window(int width, int hight, const std::string& title);

    /**
     * @brief Destructor de la ventana.
     */
    ~Window();

    /**
     * @brief Verifica si la ventana está abierta.
     * @return true si la ventana está abierta, false en caso contrario.
     */
    bool isOpen() const;

    /**
     * @brief Limpia la ventana con un color específico.
     * @param color Color de limpieza (por defecto azul).
     */
    void clear(const sf::Color& color = sf::Color(0, 0, 255));

    /**
     * @brief Dibuja un objeto en la ventana.
     * @param drawable Objeto SFML dibujable.
     * @param state    Estado de renderizado (por defecto `sf::RenderStates::Default`).
     */
    void draw(const sf::Drawable& drawable,
        const sf::RenderStates& state = sf::RenderStates::Default);

    /**
     * @brief Muestra en pantalla todo lo dibujado en el frame actual.
     */
    void display();

    /**
     * @brief Cierra la ventana.
     */
    void close();

    void handleResize(const sf::Vector2u& size);

    void applyCameraView(const sf::Vector2f& position, float zoom);


    /**
     * @brief Actualiza el estado de la ventana (eventos, lógica, etc.).
     */
    void Update();

    /**
     * @brief Renderiza el contenido de la ventana.
     */
    void render();

    /**
     * @brief Destruye los recursos asociados a la ventana.
     */
    void destroy();

    /// Puntero a la ventana SFML.
    sf::RenderWindow* m_window = nullptr;

private:
    /// Vista de la ventana (control de cámara/zoom).
    sf::View m_view;

    /// Tiempo transcurrido entre frames.
    sf::Time deltatime;

    /// Reloj para medir el tiempo de actualización.
    sf::Clock clock;
};
