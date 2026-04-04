#include <Audio.hpp>
#include <Window.hpp>
#include <globals.hpp>

sf::RenderWindow *window;
const sf::Vector2u WINDOW_SIZE = {1150, 606};

void initWindow() {
  window = new sf::RenderWindow(sf::VideoMode(WINDOW_SIZE), "Plants vs Zombies",
                                sf::Style::Close | sf::Style::Titlebar);
  window->setFramerateLimit(60);

  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  window->setPosition(sf::Vector2i((desktop.size.x - window->getSize().x) / 2,
                                   (desktop.size.y - window->getSize().y) / 2));

  sf::Image icon("assets/icon.png");
  window->setIcon(icon.getSize(), icon.getPixelsPtr());

  setCursorMain();
}

void setCursorMain() {
  static sf::Image cursorMainImage("assets/cursorMain.png");
  static sf::Cursor cursorMain(cursorMainImage.getPixelsPtr(),
                               cursorMainImage.getSize(), sf::Vector2u(3, 3));
  window->setMouseCursor(cursorMain);
}

void setCursorHover() {
  sf::Image cursorHoverImage("assets/cursorHover.png");
  static sf::Cursor cursorHover(cursorHoverImage.getPixelsPtr(),
                                cursorHoverImage.getSize(), sf::Vector2u(3, 3));
  window->setMouseCursor(cursorHover);
}

void handleEvents() {
  while (const std::optional event = window->pollEvent()) {
    if (event->is<sf::Event::Closed>()) // Close Game
      window->close();

    if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
      if (key->code == sf::Keyboard::Key::Escape &&
          gameState != 0) { // Pause / UnPause
        if (!isPaused) {
          playSound("Pause");
          gameWeather.isPaused = true;
        } else
          gameWeather.isPaused = false;

        isPaused = !isPaused;
      }
    }
  }
}
