#include "config.h"
#include "gameScreen.h"
#include "utils.h"
#include "mouseEvent.h"

GameScreen::GameScreen(Application *app) :
  Screen(app),
  hud(app->getImgMgr())
{
  game = new Game(app, this);
}

void GameScreen::onEvent(sf::Event &evt) {
  switch (evt.type) {
    case sf::Event::Closed:
      log("Closed");
      exit();
      break;
    case sf::Event::Resized:
      log("Resized");
      onResized(evt);
      break;
    case sf::Event::KeyReleased:
      log("KeyReleased");
      onKeyReleased(evt);
      break;
    case sf::Event::MouseWheelMoved:
      log("MouseWheelMoved");
      onMouseWheelMoved(evt);
      break;
    case sf::Event::MouseButtonPressed:
      log("MouseButtonPressed");
      onMouseButtonPressed(evt);
      break;
    default:
      break;
  }
}

ScreenID GameScreen::run() {
  log("GAME");

  sf::RenderWindow &window = *app->getWindow();

  nextScreen = SCREEN_THIS;
  while (nextScreen == SCREEN_THIS) {
    sf::Event evt;
    while (window.pollEvent(evt))
      onEvent(evt);

    if (nextScreen != SCREEN_THIS)
      break;

    tick(&window);
    window.display();
  }

  if (nextScreen != SCREEN_THIS)
    return nextScreen;

  return SCREEN_EXIT;
}

void GameScreen::exit() {
  nextScreen = SCREEN_EXIT;
}

void GameScreen::update() {
  game->update();
  repaint();
}

void GameScreen::paintDebug(sf::RenderTarget *target) {
  sf::Text out = sf::Text();
  out.setFont(DEFAULT_FONT);
  out.setCharacterSize(12);
  out.setColor(sf::Color::White);

  std::ostringstream s;
  sf::Vector2i cur = app->getCursorPosition();
  s << "Cursor: " << cur.x << ", " << cur.y << std::endl
    << "Radius: " << game->getRadius();
  out.setString(s.str());

  out.setPosition(target->mapPixelToCoords(sf::Vector2i(10, 10)));
  target->draw(out);
}

void GameScreen::paint(sf::RenderTarget *target) {
  target->clear(sf::Color::Black);
}

void GameScreen::tick(sf::RenderTarget *target) {
  Screen::tick(target);

#if DEBUG
  paintDebug(target);
#endif
  //TODO: unleash the hud *only* when decent graphics for it are added
  //hud.paint(target);
  app->getCursor()->paint();
}

void GameScreen::onResized(sf::Event &evt) {
  sf::RenderWindow &window = *app->getWindow();
  sf::View view = window.getView();
  view.setSize(evt.size.width, evt.size.height);
  window.setView(view);
}

void GameScreen::onKeyReleased(sf::Event &evt) {
  switch (evt.key.code) {
    case sf::Keyboard::Escape:
    case sf::Keyboard::Q:
      exit();
      break;
    case sf::Keyboard::F11:
      nextScreen = SCREEN_MENU;
      break;
    case sf::Keyboard::F10:
      app->toggleFullscreen();
      break;
    default:
      break;
  }
}

void GameScreen::onMouseWheelMoved(sf::Event &evt) {
  int radius = game->getRadius() + evt.mouseWheel.delta;
  radius = clamp(radius, 0, 10);
  game->setRadius(radius);
}

void GameScreen::onMouseButtonPressed(sf::Event &evt) {
  int button;
  sf::Vector2i coords = app->getCursor()->getPosition();

  switch (evt.mouseButton.button) {
    case sf::Mouse::Left:
      button = MouseEvent::BUTTON1;
      break;
    case sf::Mouse::Right:
      button = MouseEvent::BUTTON2;
      break;
    case sf::Mouse::Middle:
      button = MouseEvent::BUTTON3;
      break;
    default:
      button = MouseEvent::NOBUTTON;
      break;
  }

  if (button != MouseEvent::NOBUTTON) {
    MouseEvent e(MouseEvent::MOUSE_PRESSED, button, coords);
    dispatchEvent(e);
  }
}
