#include "config.h"
#include "menuScreen.h"

MenuScreen::MenuScreen(Application* app) : Screen(app) {
	// TODO: construct things
	log("MENU");
	nextScreen = SCREEN_MENU;
}

ScreenID MenuScreen::run(sf::RenderWindow* window)
{
	

	//sf::RenderWindow& window = *app->getWindow();

	running = true;
	//while (running)
	//{
	/*	sf::Event evt;
		while (window->pollEvent(evt))
		{
			
		}

		if (!running)
			break;
		*/
		tick(window);
		//window->display();
	//}

	return nextScreen;
}



void MenuScreen::update() {}

void MenuScreen::paint(sf::RenderTarget* target) {}

void MenuScreen::onEvent(sf::Event& evt)
{
	switch (evt.type)
	{
	case sf::Event::Closed:
		log("Closed");
		running = false;
		break;
	case sf::Event::KeyReleased:
		log("KeyReleased");
		switch (evt.key.code)
		{
		case sf::Keyboard::Escape:
		case sf::Keyboard::Q:
			nextScreen = SCREEN_EXIT;
			log("SCREEN_EXIT");
			break;
		case sf::Keyboard::F11:
			nextScreen = SCREEN_GAME;
			log("SCREEN_GAME");
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
