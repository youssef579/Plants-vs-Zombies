#include <SFML/Window/Keyboard.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <BackgroundManager.hpp>
#include <Packets/Packet.hpp> // USED FOR DEV MODE
#include <SunManager.hpp>     // USED FOR DEV MODE


sf::RenderWindow *window;
sf::View *view;
sf::View *gameView;
const sf::Vector2u WINDOW_SIZE = {1150, 606};

bool isMousePressed, isMouseReleased;

void initWindow() {
  window = new sf::RenderWindow(sf::VideoMode(WINDOW_SIZE), "Plants vs Zombies");

  // This is the game view
  // It scales the game while keeping the original ration
  view = new sf::View();
  view->setSize((sf::Vector2f)WINDOW_SIZE);
  view->setCenter((sf::Vector2f)WINDOW_SIZE / 2.0f);

  gameView = new sf::View();
  gameView->setSize((sf::Vector2f)WINDOW_SIZE);
  gameView->setCenter((sf::Vector2f)WINDOW_SIZE / 2.0f);


  getLetterboxView(WINDOW_SIZE.x, WINDOW_SIZE.y);
}

void setWindowMetaData() { // Set icon, cursor and window settings after creating a window
  window->setFramerateLimit(144);

  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  window->setPosition(sf::Vector2i((desktop.size.x - window->getSize().x) / 2, (desktop.size.y - window->getSize().y) / 2));

  sf::Image icon("assets/icon.png");
  window->setIcon(icon.getSize(), icon.getPixelsPtr());
  setCursorMain();
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
    dayLevel.camera.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
    gameView->setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
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
  //static bool isFullscreen = false;

  isMousePressed = isMouseReleased = false;
  while (const std::optional event = window->pollEvent())
  {
    if (event->is<sf::Event::Closed>()) // Close Game
      window->close();

    if (const auto* resized = event->getIf<sf::Event::Resized>()) { // Resize game when window is resized
      getLetterboxView(resized->size.x, resized->size.y);
    }

    if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
      if (mousePress->button == sf::Mouse::Button::Left)
        isMousePressed = true;
    }

    if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
      if (mouseRelease->button == sf::Mouse::Button::Left)
        isMouseReleased = true;
    }

    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
      switch (keyPress->code) {
      case sf::Keyboard::Key::F11:
        //isFullscreen = !isFullscreen;
        settings.fullscreen = !settings.fullscreen;

        if (settings.fullscreen) {
          window->create(sf::VideoMode::getDesktopMode(), "Plants vs Zombies", sf::Style::None, sf::State::Fullscreen);
          view->setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
          dayLevel.camera.setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
          gameView->setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        }
        else {
          window->create(sf::VideoMode(WINDOW_SIZE), "Plants vs Zombies"); // Default is windowed
          getLetterboxView(window->getSize().x, window->getSize().y);
        }
        setWindowMetaData();

        break;

      case sf::Keyboard::Key::Escape:
        if (gameState != 0 && dayLevel.state != dayLevel.GameOver) { // Pause / UnPause
          if (!isPaused) {
            sounds.play("Pause");
            gameWeather.isPaused = true;
            setCursorMain();
          }
          else gameWeather.isPaused = false;
          gameWeather.update(0);

          isPaused = !isPaused;
        }
        break;
      case sf::Keyboard::Key::Tab:
        if(gameState != 0 && !isPaused)
          settings.timeModifier = (settings.timeModifier % 3) + 1; // cycle between {1, 2, 3}
        break;
      case sf::Keyboard::Key::Grave: { // DEV MODE
        Array<PlantType> types;
        for (auto t : { PEASHOOTER, SUN_FLOWER, WALLNUT, SNOWPEASHOOTER,
          REPEATERPEA, TALLNUT, CHERRYBOMB, JALAPENO, POTATOMINE, ICESHROOM, SQUASH })
          types.push(t);
        fillPackets(types);
        Sun::sunBalance = 5000;
        for (int i = 0; i < packets.size; i++)
          packets[i].reloadDuration = 1.0f;
        break;
      }
      //case sf::Keyboard::Key::J: // REMOVE LATER
      //  testKeybinds("j");
      //  break;
      //case sf::Keyboard::Key::K: // REMOVE LATER
      //  testKeybinds("k");
      //  break;
      //case sf::Keyboard::Key::I: // REMOVE LATER
      //  testKeybinds("i");
      //  break;
      //case sf::Keyboard::Key::M: // REMOVE LATER
      //  testKeybinds("m");
      //  break;
      //case sf::Keyboard::Key::Up:
      //  testKeybinds("up");
      //  break;
      //case sf::Keyboard::Key::Down:
      //  testKeybinds("down");
      //  break;
      //case sf::Keyboard::Key::Left:
      //  testKeybinds("left");
      //  break;
      //case sf::Keyboard::Key::Right:
      //  testKeybinds("right");
      //  break;
      //case sf::Keyboard::Key::RShift:
      //  testKeybinds("sw");
      //  break;
      }
    }
  }
}
