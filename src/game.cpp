#include <SFML/Window/Keyboard.hpp>
#include "config.h"
#include "game.h"

Game::Game() {
  window = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Tiled Map");
  window->SetPosition(0, 0);

  imageMgr = new ImageManager();
  cursor = new Cursor(window, imageMgr);
  map = Map::fromFile(res_path("map"), imageMgr);
}

Game::~Game() {
  delete cursor;
  delete imageMgr;
}

void Game::onEvent(sf::Event &evt) {
  switch (evt.Type) {
    case sf::Event::Closed:
      log("Closed");
      exit();
      break;
    case sf::Event::MouseMoved:
      log("MouseMoved");
      onMouseMoved(evt);
      break;
    case sf::Event::KeyReleased:
      log("KeyReleased");
      onKeyReleased(evt);
      break;
    default:
      log("Unhandled");
      break;
  }
}

void Game::run() {
  window->EnableKeyRepeat(false);
  running = true;
  while (running) {
    sf::Event evt;
    while (window->PollEvent(evt)) {
      onEvent(evt);
    }

    if (!running)
      break;

    paint();
    window->Display();
  }
}

void Game::exit() {
  running = false;
  window->Close();
}

void Game::paintDebug() {
  sf::Text out = sf::Text();
  out.SetCharacterSize(12);
  out.SetColor(sf::Color::White);

  std::ostringstream s;
  sf::Vector2i cur = cursor->getPosition();
  s << "Cursor: " << cur.x << ", " << cur.y;
  out.SetString(s.str());

  out.SetPosition(window->ConvertCoords(0, 0));
  window->Draw(out);
}

void Game::paint() {
  window->Clear(sf::Color::Black);
  map->paint(window);
#if DEBUG
  paintDebug();
#endif
  cursor->paint();
}

void Game::onMouseMoved(sf::Event &evt) {
  cursor->onMoved(evt.MouseMove);
}

void Game::onKeyReleased(sf::Event &evt) {
  switch (evt.Key.Code) {
    case sf::Keyboard::Escape:
    case sf::Keyboard::Q:
      exit();
      break;
    default:
      break;
  }
}
