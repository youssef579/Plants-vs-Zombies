#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Overlay.hpp>
#include <SunManager.hpp>
#include <UI/Checkbox.hpp>
#include <UI/Pause.hpp>
#include <UI/Slider.hpp>
#include <Weather.hpp>
#include <globals.hpp>

void updatePause() {
  static sf::Texture &pauseMenuTexture = getTexture("assets/pause-menu.png");
  static sf::Sprite pauseMenuSprite(pauseMenuTexture);
  static bool runOnce = true;

  // Back to Game button
  static sf::Text pauseMenuBacktoGameButton(assets->font, "Back To Game", 40);
  onClick(pauseMenuBacktoGameButton, []() {
    playSound("ButtonClick");
    gameWeather.isPaused = false;
    isPaused = false;
  });

  // Main Menu Button
  static sf::Text pauseMenuMainMenuButton(assets->font, "Main Menu", 20);
  onClick(pauseMenuMainMenuButton, []() {
    gameState = 0;
    homeState = 0; // go to home menu not level selector
    playSound("ButtonClick");
    playMusic("Menu");
    isPaused = false;
  }); // TODO: Handle resetting level data (currently aknk 2flt w rg3t btkml
      // mkank)

  // Restart Level Button
  static sf::Text pauseMenuRestartLevelButton(assets->font, "Restart Level",
                                              20);
  onClick(pauseMenuRestartLevelButton, []() {}); // TODO: Add restartLevel()

  static sf::Texture &pauseMenuSliderTexture = getTexture("assets/slider.png");
  // Music Slider
  static sf::Sprite pauseMenuMusicSliderSprite(pauseMenuSliderTexture);
  static Slider pauseMenuMusicSlider = {
      pauseMenuMusicSliderSprite, 178.0f, 562.0f, 676.0f, 114.0f, false};
  // SoundFX Slider
  static sf::Sprite pauseMenuSoundFXSliderSprite(pauseMenuSliderTexture);
  static Slider pauseMenuSoundFXSlider = {
      pauseMenuSoundFXSliderSprite, 205.0f, 562.0f, 676.0f, 114.0f, false};
  // WeatherFX Slider
  static sf::Sprite pauseMenuWeatherFXSliderSprite(pauseMenuSliderTexture);
  static Slider pauseMenuWeatherFXSlider = {
      pauseMenuWeatherFXSliderSprite, 232.0f, 562.0f, 676.0f, 114.0f, false};

  static sf::Texture &pauseMenuCheckboxTexture =
      getTexture("assets/checkbox.png");
  static sf::Texture &pauseMenuCheckmarkTexture =
      getTexture("assets/checkmark.png");
  // Weather Active Chechbox
  static sf::Sprite pauseMenuWeatherActiveCheckboxSprite(
      pauseMenuCheckboxTexture);
  static sf::Sprite pauseMenuWeatherActiveCheckmark(pauseMenuCheckmarkTexture);
  static Checkbox pauseMenuWeatherActiveCheckbox = {
      pauseMenuWeatherActiveCheckboxSprite, pauseMenuWeatherActiveCheckmark,
      639.0f, 258.0f, settingsWeatherActive};

  if (runOnce) {
    pauseMenuSprite.setPosition({358.5, 54.5});
    // Back to Game
    pauseMenuBacktoGameButton.setPosition({428, 455});
    pauseMenuBacktoGameButton.setFillColor({0, 196, 0});
    pauseMenuBacktoGameButton.setStyle(sf::Text::Bold);
    pauseMenuBacktoGameButton.setOutlineColor(sf::Color::Black);
    pauseMenuBacktoGameButton.setOutlineThickness(1.0);
    // Main Menu
    pauseMenuMainMenuButton.setFillColor({0, 196, 0});
    pauseMenuMainMenuButton.setPosition({515, 389});
    pauseMenuMainMenuButton.setOutlineColor(sf::Color::Black);
    pauseMenuMainMenuButton.setOutlineThickness(1.0);
    // Restart Level
    pauseMenuRestartLevelButton.setFillColor({0, 196, 0});
    pauseMenuRestartLevelButton.setPosition({510, 346});
    pauseMenuRestartLevelButton.setOutlineColor(sf::Color::Black);
    pauseMenuRestartLevelButton.setOutlineThickness(1.0);
    // Music Slider
    pauseMenuMusicSlider.sprite.setPosition(
        {pauseMenuMusicSlider.lowerBound +
             settingsMusicVolume * pauseMenuMusicSlider.length / 100.0f,
         pauseMenuMusicSlider.y}); // Min Bound
    // Sound FX Slider
    pauseMenuSoundFXSlider.sprite.setPosition(
        {pauseMenuSoundFXSlider.lowerBound +
             settingsSoundFXVolume * pauseMenuSoundFXSlider.length / 100.0f,
         pauseMenuSoundFXSlider.y}); // Min Bound
    // Weather FX Slider
    pauseMenuWeatherFXSlider.sprite.setPosition(
        {pauseMenuWeatherFXSlider.lowerBound +
             settingsWeatherFXVolume * pauseMenuWeatherFXSlider.length / 100.0f,
         pauseMenuWeatherFXSlider.y}); // Min Bound

    // Weather Active Checkbox
    pauseMenuWeatherActiveCheckbox.box.setPosition(
        {pauseMenuWeatherActiveCheckbox.x, pauseMenuWeatherActiveCheckbox.y});
    pauseMenuWeatherActiveCheckbox.mark.setPosition(
        {pauseMenuWeatherActiveCheckbox.x + 5.0f,
         pauseMenuWeatherActiveCheckbox.y - 4.0f});

    runOnce = false;
  }

  settingsMusicVolume = updateSlider(pauseMenuMusicSlider);
  settingsSoundFXVolume = updateSlider(pauseMenuSoundFXSlider);
  settingsWeatherFXVolume = updateSlider(pauseMenuWeatherFXSlider);

  updateCheckbox(pauseMenuWeatherActiveCheckbox, settingsWeatherActive);

  updateVolume(&gameWeather);

  // drawUI();
  drawSun();

  window->draw(overlay->overlayRect);
  window->draw(pauseMenuSprite);

  // Buttons
  window->draw(pauseMenuBacktoGameButton);
  window->draw(pauseMenuMainMenuButton);
  window->draw(pauseMenuRestartLevelButton);
  // Sliders
  window->draw(pauseMenuMusicSlider.sprite);
  window->draw(pauseMenuSoundFXSlider.sprite);
  window->draw(pauseMenuWeatherFXSlider.sprite);
  // Checkboxes & Checkmarks
  window->draw(pauseMenuWeatherActiveCheckbox.box);
  if (pauseMenuWeatherActiveCheckbox.checked)
    window->draw(pauseMenuWeatherActiveCheckbox.mark);
}
