#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Game.hpp>
#include <Home.hpp>
#include <Overlay.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <Weather.hpp>
#include <SunManager.hpp>

int gameState = 0;
/*
  0 -> Home menu
*/

//  a8lb el const mmkn yt3mlo hard-coded b3d ma netefe2 3la el values bs
//  ma7toten ka placeholder 3shan yb2o more accessible


sf::Clock drawClock;
float dt; // Delta Time (time between each frame draw)

static WeatherSystem homeWeather;
static bool weatherInited = false;

bool isPaused = false;
bool runOnce = true;
bool runOncePause = true;
bool runOnceUI = true;
void updateGame() {
  dt = drawClock.restart()
           .asSeconds(); // clock.restart() sets time to 0 and returns the last
                         // time before modifying it
  // calling dt = clock.restart() each frame returns the time between frames
  // (dt)

  switch (gameState) {
  case 0:
    updateHome();
    break;
  default:
    // Replace with loadLevel() w copy kol el logic hnak


    //Weather System
    static float x = 0;
    if (!weatherInited) {
      //window->setFramerateLimit(60);
      homeWeather.init(window->getSize());
      weatherInited = true;
      homeWeather.isRaining = true;
    }
    homeWeather.update(window->getSize());

    x += dt;
    if (x >= 7.0) homeWeather.isRaining = false;


    if (isPaused) {
      updatePause();
      break;
    }

    if (runOnce) {
      playMusic("DayStage");
      runOnce = false;
    }

    manageSuns(dt);
    drawUI();
    break;
  }
}



//void animateFrames(sf::Sprite &sprite, sf::Texture *frames[], int nFrames,
//                   int &currentFrame, float &timer) {
//  timer -= dt;
//  if (timer <= 0) {
//    currentFrame = (currentFrame + 1) % nFrames;
//    sprite.setTexture(*frames[currentFrame]);
//    timer = 0.03;
//  }
//};




void updatePause() {
  static sf::Texture &PauseMenuTexture = getTexture("assets/pause-menu.png");
  static sf::Sprite PauseMenuSprite(PauseMenuTexture);

  // Back to Game button
  static sf::Text PauseMenuBacktoGameButton(assets->font, "Back To Game", 40);
  onClick(PauseMenuBacktoGameButton, []() { isPaused = false; });

  // Main Menu Button
  static sf::Text PauseMenuMainMenuButton(assets->font, "Main Menu", 20);
  onClick(PauseMenuMainMenuButton, []() {
    gameState = 0;
    homeState = 0; //go to home menu not level selector
    playMusic("Menu");
    isPaused = false;
  }); // TODO: Handle resetting level data (currently aknk 2flt w rg3t btkml
      // mkank)

  // Restart Level Button
  static sf::Text PauseMenuRestartLevelButton(assets->font, "Restart Level",
                                              20);
  onClick(PauseMenuRestartLevelButton, []() {}); // TODO: Add restartLevel()

  static sf::Texture &PauseMenuSliderTexture = getTexture("assets/slider.png");
  // Music Slider
  static sf::Sprite PauseMenuMusicSliderSprite(PauseMenuSliderTexture);
  static Slider PauseMenuMusicSlider = {
      PauseMenuMusicSliderSprite, 178.0f, 566.0f, 676.0f, 110.0f, false};
  // SoundFX Slider
  static sf::Sprite PauseMenuSoundFXSliderSprite(PauseMenuSliderTexture);
  static Slider PauseMenuSoundFXSlider = {
      PauseMenuSoundFXSliderSprite, 205.0f, 566.0f, 676.0f, 110.0f, false};

  if (runOncePause) {
    PauseMenuSprite.setPosition({358.5, 54.5});
    // Back to Game
    PauseMenuBacktoGameButton.setPosition({428, 455});
    PauseMenuBacktoGameButton.setFillColor({0, 196, 0});
    PauseMenuBacktoGameButton.setStyle(sf::Text::Bold);
    PauseMenuBacktoGameButton.setOutlineColor(sf::Color::Black);
    PauseMenuBacktoGameButton.setOutlineThickness(1.0);
    // Main Menu
    PauseMenuMainMenuButton.setFillColor({0, 196, 0});
    PauseMenuMainMenuButton.setPosition({515, 389});
    PauseMenuMainMenuButton.setOutlineColor(sf::Color::Black);
    PauseMenuMainMenuButton.setOutlineThickness(1.0);
    // Restart Level
    PauseMenuRestartLevelButton.setFillColor({0, 196, 0});
    PauseMenuRestartLevelButton.setPosition({510, 346});
    PauseMenuRestartLevelButton.setOutlineColor(sf::Color::Black);
    PauseMenuRestartLevelButton.setOutlineThickness(1.0);
    // Music Slider
    PauseMenuMusicSlider.sprite.setPosition(
        {PauseMenuMusicSlider.lowerBound +
             Settings_MusicVolume * PauseMenuMusicSlider.length / 100.0f,
         PauseMenuMusicSlider.y}); // Min Bound
    // Sound FX Slider
    PauseMenuSoundFXSlider.sprite.setPosition(
        {PauseMenuSoundFXSlider.lowerBound +
             Settings_SoundFXVolume * PauseMenuSoundFXSlider.length / 100.0f,
         PauseMenuSoundFXSlider.y}); // Min Bound

    runOncePause = false;
  }

  Settings_MusicVolume = updateSlider(PauseMenuMusicSlider);
  Settings_SoundFXVolume = updateSlider(PauseMenuSoundFXSlider);

  updateVolume(Settings_MusicVolume, Settings_SoundFXVolume);

  drawUI();
  drawSun();

  window->draw(overlay->overlayRect);
  window->draw(PauseMenuSprite);
  window->draw(PauseMenuBacktoGameButton);
  window->draw(PauseMenuMainMenuButton);
  window->draw(PauseMenuRestartLevelButton);
  window->draw(PauseMenuMusicSlider.sprite);
  window->draw(PauseMenuSoundFXSlider.sprite);
}

void drawUI() {
  static sf::Text SunBalanceText(assets->font, std::to_string(SunBalance), 40);
  static sf::Texture& sunBankTexture = getTexture("assets/Sun/sun_bank.png");
  static sf::Sprite sunBank(sunBankTexture);

  SunBalanceText.setString(std::to_string(SunBalance));
  SunBalanceText.setOrigin(SunBalanceText.getLocalBounds().getCenter()); //re-center text

  if (runOnceUI) {

    SunBalanceText.setPosition({37.0, 72.0});
    SunBalanceText.setFillColor({0, 0, 0, 255});
    SunBalanceText.setOutlineColor({255, 255, 255, 255});
    SunBalanceText.setOutlineThickness(1.0f);
    SunBalanceText.setCharacterSize(20.0f);
    SunBalanceText.setStyle(sf::Text::Style::Regular);
    sunBank.setPosition({0, 0});
    runOnceUI = false;
  }


  homeWeather.draw(*window);

  window->draw(sunBank); // Draw order matters !!
  window->draw(SunBalanceText);
}
