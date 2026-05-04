#pragma once
#include <SFML/Graphics.hpp>
#include <AssetsManager.hpp>
#include <globals.hpp>
#include <Audio.hpp>
#include <Window.hpp>
#include <UI/Overlay.hpp>
#include <UI/Draw.hpp>
#include <UI/UI.hpp>
#include <Files.hpp>
#include <Packets/Packet.hpp>
#include <Packets/Shovel.hpp>
#include <BackgroundManager.hpp>

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

  //options menu
  sf::Text* doneBtn;
  sf::Sprite* backBtn;
  sf::Texture backgroundOptionsT;
  sf::Sprite* backgroundOptionsS;

  sf::Texture sliderKnobT;
  sf::Sprite* sliderMusicS;
  Slider* sliderMusic;
  sf::Sprite* sliderSFXS;
  Slider* sliderSFX;
  sf::Sprite* sliderWeatherFXS;
  Slider* sliderWeatherFX;

  sf::Texture checkboxBoxT;
  sf::Texture checkboxMarkT;

  sf::Sprite* checkboxWeatherActiveS;
  sf::Sprite* checkboxWeatherActiveM;
  Checkbox* checkboxWeatherActive;

  sf::Sprite* checkboxFullscreenS;
  sf::Sprite* checkboxFullscreenM;
  Checkbox* checkboxFullscreen;

  bool resHovered = false;
  bool restartHovered = false;
  bool settingsHovered = false;
  bool menuHovered = false;
  bool backHovered = false;

  bool isOpen = false;

  bool isSettingsOpen = false;


  void init();
  void update(sf::RenderWindow& window);
  void draw(sf::RenderWindow& window);


  bool handleButtonLogic(sf::Sprite* btn, sf::Vector2f mousePos, bool& actionTriggered, bool& isHovered, float baseScale = 0.98f);
};

extern newPauseMenu newPause;
