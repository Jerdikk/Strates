#include <SFML/Window/Keyboard.hpp>
#include "config.h"
#include "game.h"
#include "utils.h"

Game::Game() {
  window = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Tiled Map");
  window->SetPosition(0, 0);

  imageMgr = new ImageManager();
  cursor = new Cursor(window, imageMgr);
  map = Map::fromFile(res_path("map"), imageMgr);
  fog = new FoW(map, imageMgr);
}

Game::~Game() {
  delete cursor;
  delete imageMgr;
  delete map;
  delete fog;
}

void Game::onEvent(sf::Event &evt) {
  switch (evt.Type) {
    case sf::Event::Closed:
      log("Closed");
      exit();
      break;
    case sf::Event::KeyReleased:
      log("KeyReleased");
      onKeyReleased(evt);
      break;
    case sf::Event::MouseButtonPressed:
      log("MouseButtonPressed");
      onMouseButtonPressed(evt);
      break;
    default:
      break;
  }
}

void Game::run() {
  window->EnableVerticalSync(true);
  window->SetFramerateLimit(FRAMERATE_LIMIT);
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

void Game::update() {
  int scrollSpeed = 5;
  sf::View view = window->GetView();
  if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Left))
    view.Move(-scrollSpeed, 0);
  if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Right))
    view.Move(scrollSpeed, 0);
  if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Up))
    view.Move(0, -scrollSpeed);
  if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Down))
    view.Move(0, scrollSpeed);

  sf::IntRect rect = viewGetRect(view);
  rect = map->clampViewRect(rect);
  viewSetRect(view, rect);
  window->SetView(view);
}

void Game::paintDebug() {
  sf::Text out = sf::Text();
  out.SetCharacterSize(12);
  out.SetColor(sf::Color::White);

  std::ostringstream s;
  sf::Vector2i cur = cursor->getPosition();
  s << "FPS: " << 1000. / window->GetFrameTime() << std::endl
    << "Cursor: " << cur.x << ", " << cur.y;
  out.SetString(s.str());

  out.SetPosition(window->ConvertCoords(0, 0));
  window->Draw(out);
}

void Game::select() {
  sf::Vector2i curPos = (sf::Vector2i) cursor->getViewPosition();
  sf::Vector2i mapPos = map->viewToMapCoords(curPos);
  sf::IntRect rect = map->mapToViewRect(mapPos);
  sf::Shape selection = sf::Shape::Rectangle(rect.Left + 1, rect.Top + 1, rect.Width - 2, rect.Height - 2, sf::Color::White, 1, sf::Color::Green);
  selection.EnableFill(false);
  window->Draw(selection);
}

void Game::paint() {
  window->Clear(sf::Color::Black);
  map->paint(window);
  fog->paint(window);
  select();
#if DEBUG
  paintDebug();
#endif
  cursor->paint();
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

void Game::onMouseButtonPressed(sf::Event &evt) {
  sf::Vector2i coords = cursor->getPosition();
  switch (evt.MouseButton.Button) {
    case sf::Mouse::Left:
      fog->set(map->viewToMapCoords(coords), FoW::REVEALED);
      break;
    case sf::Mouse::Right:
      fog->set(map->viewToMapCoords(coords), FoW::HIDDEN);
      break;
    default:
      break;
  }
}
