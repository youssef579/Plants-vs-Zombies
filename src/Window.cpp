#include <Window.hpp>
#include <Audio.hpp>
#include <SFML/Graphics.hpp>
#include <globals.hpp>

sf::RenderWindow *window;
sf::View *view;
const sf::Vector2u WINDOW_SIZE = {1150, 606};

void initWindow() {
  window = new sf::RenderWindow(sf::VideoMode(WINDOW_SIZE), "Plants vs Zombies");
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

  view = new sf::View();
  view->setSize((sf::Vector2f)WINDOW_SIZE);
  view->setCenter((sf::Vector2f)WINDOW_SIZE / 2.0f);

  getLetterboxView(WINDOW_SIZE.x, WINDOW_SIZE.y);
}

void getLetterboxView(int windowWidth, int windowHeight) {
    float windowRatio = windowWidth / (float)windowHeight;
    float viewRatio = view->getSize().x / (float)view->getSize().y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    if (windowRatio >= viewRatio) { // If window is wider than view, add black bars on sides (Pillarbox)
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    } else { // If window is taller than view, add black bars on top/bottom (Letterbox)
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }

    view->setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
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


void handleEvents() {
  while (const std::optional event = window->pollEvent())
  {
    if (event->is<sf::Event::Closed>()) //Close Game
      window->close();

    if (const auto* resized = event->getIf<sf::Event::Resized>())
      getLetterboxView(resized->size.x, resized->size.y);

    if (const auto* key = event->getIf<sf::Event::KeyPressed>()) //Key Presses
    {
      if (key->code == sf::Keyboard::Key::Escape && gameState != 0) //Pause / UnPause
      {
        if (!isPaused) {
          sounds.play("Pause");
          gameWeather.isPaused = true;
          setCursorMain();
        }
        else gameWeather.isPaused = false;
        gameWeather.update(0);

        isPaused = !isPaused;
      }
    }

  }
}
