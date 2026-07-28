#pragma once
#include "Prerequisitos.h"

class
	Window {
public:
	Window() = default;
	Window(int width, int height, const std::string& title);
	~Window() = default;

	//void 
	//handleEvents(EngineGUI& engineGUI);

	bool
		isOpen() const;

	void
		clear(const sf::Color& color = sf::Color(0, 0, 0, 255));

	void
		draw(const sf::Drawable& drawable,
			const sf::RenderStates& states = sf::RenderStates::Default);

	void
		display();

	void
		close();

	// Reajusta la sf::View al nuevo tamaño de la ventana.
	// Llamar al recibir un sf::Event::Resized.
	void
		handleResize(const sf::Vector2u& size);

	// Aplica la vista de cámara: centra en 'center', aplica 'zoom'
	// sobre el tamaño base y rota la vista 'rotationDeg' grados.
	// La llama el CameraSystem.
	void 
		applyCameraView(const sf::Vector2f& center, float zoom, float rotationDeg = 0.f);


	/* Establece el nivel de MSAA para la ventana. */
	void
		setMSAALevel(unsigned int level);

	void
		update();

	void
		render();

	void
		destroy();

	sf::RenderWindow* getWindow() { return m_window.get(); }
	const sf::RenderWindow* getWindow() const { return m_window.get(); }

public:
	std::unique_ptr<sf::RenderWindow> m_window = nullptr;
private:
	sf::View m_view;
	sf::Vector2f m_baseViewSize{};   // tamaño de la vista sin zoom (lo fija el resize)
	sf::Time deltaTime;
	sf::Clock clock;
	std::string m_title;
};