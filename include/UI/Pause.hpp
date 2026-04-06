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

struct PauseMenu {
  sf::Texture backgroundT;
  sf::Sprite *backgroundS;
  sf::Text *backToGameBtn;
  sf::Text *mainMenuBtn;
  sf::Text *restartLevelBtn;
  sf::Texture sliderKnobT;
  sf::Sprite *sliderMusicS;
  Slider *sliderMusic;
  sf::Sprite *sliderSFXS;
  Slider *sliderSFX;
  sf::Sprite *sliderWeatherFXS;
  Slider *sliderWeatherFX;
  sf::Texture checkboxBoxT;
  sf::Texture checkboxMarkT;
  sf::Sprite *checkboxWeatherActiveS;
  sf::Sprite *checkboxWeatherActiveM;
  Checkbox *checkboxWeatherActive;


  PauseMenu();
  void init();
  void update();
  void draw();

};



extern PauseMenu pauseMenu;
