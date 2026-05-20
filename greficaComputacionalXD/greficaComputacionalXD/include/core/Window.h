#include "Prerequisitos.h"
class Window {
public:
	Window() = default;
	Window(int  width, int hight, const std::string& title);
	~Window() {
		SAFE_PTR_RELEASE(m_window);
	};

	//void handleEvents(EngineGui& engineGui);

	bool 
	isOpen() const;

	void 
	clear(const sf::Color& color = sf::Color(0, 0, 255));

	void 
	draw(const sf::Drawable& drawable,
		const sf::RenderStates& state = sf::RenderStates::Default);

	void 
	display();

	void 
	close();

	void 
	Update();

	void 
	render();

	void 
	destroy();

private:
	sf::View m_view;
	sf::RenderWindow* m_window; 
	sf::Time deltatime;
	sf::Clock clock;

};