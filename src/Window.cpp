#include <Window.hpp>

sf::RenderWindow *window;

void initWindow() {
  window = new sf::RenderWindow(sf::VideoMode({1150, 606}), "Plants vs Zombies",
                                sf::Style::Close | sf::Style::Titlebar);
  window->setFramerateLimit(60);

  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  window->setPosition(sf::Vector2i((desktop.size.x - window->getSize().x) / 2,
                                   (desktop.size.y - window->getSize().y) / 2));

  sf::Image icon("assets/icon.png");
  window->setIcon(icon.getSize(), icon.getPixelsPtr());

  sf::Image cursorImage("assets/cursorMain.png");
  static sf::Cursor cursor(cursorImage.getPixelsPtr(), cursorImage.getSize(),
                           sf::Vector2u(3, 3));
  window->setMouseCursor(cursor);
}


void setCursorMain() {
  static sf::Image cursorMainImage("assets/cursorMain.png");
  static sf::Cursor cursorMain(cursorMainImage.getPixelsPtr(), cursorMainImage.getSize(),
    sf::Vector2u(3, 3));
  window->setMouseCursor(cursorMain);
}

void setCursorHover() {
  sf::Image cursorHoverImage("assets/cursorHover.png");
  static sf::Cursor cursorHover(cursorHoverImage.getPixelsPtr(), cursorHoverImage.getSize(),
    sf::Vector2u(3, 3));
  window->setMouseCursor(cursorHover);
}