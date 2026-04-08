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
#include <LevelManager.hpp>


int gameState = 0;
/*
  0 -> Home menu
*/

sf::Clock drawClock;
float dt; // Delta Time (time between each frame draw)

//static WeatherSystem gameWeather;
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
    
    if (isPaused) {
      updatePause();
      break;
    }

      //gameWeather.isRaining = true;
    if (runOnce) {
      dayLevel.init();
      gameWeather.isRaining = true;
      /*gameWeather.init(window->getSize());
      weatherInited = true;
      gameWeather.isRaining = true;*/

      playMusic("DayStage");
      runOnce = false;
    }

    dayLevel.update(dt);
    dayLevel.draw(*window);

    manageSuns(dt);
    gameWeather.update(dt);
    drawUI();
    break;
  }
}


void updatePause() {
  static sf::Texture &PauseMenuTexture = getTexture("assets/pause-menu.png");
  static sf::Sprite PauseMenuSprite(PauseMenuTexture);

  // Back to Game button
  static sf::Text PauseMenuBacktoGameButton(assets->font, "Back To Game", 40);
  onClick(PauseMenuBacktoGameButton, []() {
    playSound("ButtonClick");
    setCursorMain();
    gameWeather.isPaused = false;
    isPaused = false; });

  // Main Menu Button
  static sf::Text PauseMenuMainMenuButton(assets->font, "Main Menu", 20);
  onClick(PauseMenuMainMenuButton, []() {
    gameState = 0;
    homeState = 0; //go to home menu not level selector
    playSound("ButtonClick");
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
      PauseMenuMusicSliderSprite, 178.0f, 562.0f, 676.0f, 114.0f, false};
  // SoundFX Slider
  static sf::Sprite PauseMenuSoundFXSliderSprite(PauseMenuSliderTexture);
  static Slider PauseMenuSoundFXSlider = {
      PauseMenuSoundFXSliderSprite, 205.0f, 562.0f, 676.0f, 114.0f, false};
  // WeatherFX Slider
  static sf::Sprite PauseMenuWeatherFXSliderSprite(PauseMenuSliderTexture);
  static Slider PauseMenuWeatherFXSlider = {
      PauseMenuWeatherFXSliderSprite, 232.0f, 562.0f, 676.0f, 114.0f, false };


  static sf::Texture& PauseMenuCheckboxTexture = getTexture("assets/checkbox.png");
  static sf::Texture& PauseMenuCheckmarkTexture = getTexture("assets/checkmark.png");
  //Weather Active Chechbox
  static sf::Sprite PauseMenuWeatherActiveCheckboxSprite(PauseMenuCheckboxTexture);
  static sf::Sprite PauseMenuWeatherActiveCheckmark(PauseMenuCheckmarkTexture);
  static Checkbox PauseMenuWeatherActiveCheckbox = { PauseMenuWeatherActiveCheckboxSprite ,
  PauseMenuWeatherActiveCheckmark, 639.0f, 258.0f, Settings_WeatherActive};

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
      { PauseMenuSoundFXSlider.lowerBound +
           Settings_SoundFXVolume * PauseMenuSoundFXSlider.length / 100.0f,
       PauseMenuSoundFXSlider.y }); // Min Bound
    // Weather FX Slider
    PauseMenuWeatherFXSlider.sprite.setPosition(
      { PauseMenuWeatherFXSlider.lowerBound +
           Settings_WeatherFXVolume * PauseMenuWeatherFXSlider.length / 100.0f,
       PauseMenuWeatherFXSlider.y }); // Min Bound
    
    //Weather Active Checkbox
    PauseMenuWeatherActiveCheckbox.box.setPosition({
      PauseMenuWeatherActiveCheckbox.x, PauseMenuWeatherActiveCheckbox.y });
    PauseMenuWeatherActiveCheckbox.mark.setPosition({
      PauseMenuWeatherActiveCheckbox.x+5.0f, PauseMenuWeatherActiveCheckbox.y-4.0f });


    runOncePause = false;
  }

  Settings_MusicVolume = updateSlider(PauseMenuMusicSlider);
  Settings_SoundFXVolume = updateSlider(PauseMenuSoundFXSlider);
  Settings_WeatherFXVolume = updateSlider(PauseMenuWeatherFXSlider);

  updateCheckbox(PauseMenuWeatherActiveCheckbox, Settings_WeatherActive);

  updateVolume(&gameWeather);

  drawUI();
  drawSun();

  window->draw(overlay->overlayRect);
  window->draw(PauseMenuSprite);

  //Buttons
  window->draw(PauseMenuBacktoGameButton);
  window->draw(PauseMenuMainMenuButton);
  window->draw(PauseMenuRestartLevelButton);
  //Sliders
  window->draw(PauseMenuMusicSlider.sprite);
  window->draw(PauseMenuSoundFXSlider.sprite);
  window->draw(PauseMenuWeatherFXSlider.sprite);
  //Checkboxes & Checkmarks
  window->draw(PauseMenuWeatherActiveCheckbox.box);
  if (PauseMenuWeatherActiveCheckbox.checked) window->draw(PauseMenuWeatherActiveCheckbox.mark);
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


  gameWeather.draw(*window);

  window->draw(sunBank); // Draw order matters !!
  window->draw(SunBalanceText);
}
