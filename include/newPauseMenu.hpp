#pragma once
#include <SFML/Graphics.hpp>
#include <AssetsManager.hpp>

struct newPauseMenu {

  sf::Sprite* pauseMenu = nullptr;
  sf::Sprite* resumeBtn = nullptr;
  sf::Sprite* restartBtn = nullptr;
  sf::Sprite* settingsBtn = nullptr;
  sf::Sprite* menuBtn = nullptr;
  sf::Sprite* menuPea = nullptr;
  sf::Sprite* menuSunflower = nullptr;
  sf::Sprite* menuShieldSun = nullptr;
  sf::Sprite* menuWallnut = nullptr;

  sf::SoundBuffer* hoverBuffer = nullptr;
  sf::SoundBuffer* clickBuffer = nullptr;
  sf::Sound* hoverSound = nullptr;
  sf::Sound* clickSound = nullptr;

  sf::Clock animClock;

  sf::RectangleShape overlay;

  bool resHovered = false;
  bool restartHovered = false;
  bool settingsHovered = false;
  bool menuHovered = false;

  bool isOpen = false;

  void init();
  void update(sf::RenderWindow& window);
  void draw(sf::RenderWindow& window);


  void handleButtonLogic(sf::Sprite* btn, sf::Vector2f mousePos, bool& actionTriggered, bool& isHovered);
};

extern newPauseMenu newPause ;
