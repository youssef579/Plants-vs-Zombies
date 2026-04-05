#include <Pause.hpp>

PauseMenu pauseMenu;

PauseMenu::PauseMenu() {}

void PauseMenu::init() {
  //runOnce
  backgroundT = getTexture("assets/pause-menu.png");
  backgroundS = new sf::Sprite(backgroundT);

  backToGameBtn = new sf::Text(assets->font, "Back To Game", 40);
  mainMenuBtn = new sf::Text(assets->font, "Main Menu", 20);
  restartLevelBtn = new sf::Text(assets->font, "Restart Level", 20);
  sliderKnobT = getTexture("assets/slider.png");
  sliderMusicS = new sf::Sprite(sliderKnobT);
  sliderMusic = new Slider({
      *sliderMusicS, 178.0f, 562.0f, 676.0f, 114.0f, false });
  sliderSFXS = new sf::Sprite(sliderKnobT);
  sliderSFX = new Slider({
      *sliderSFXS, 205.0f, 562.0f, 676.0f, 114.0f, false });
  sliderWeatherFXS = new sf::Sprite(sliderKnobT);
  sliderWeatherFX = new Slider({
      *sliderWeatherFXS, 232.0f, 562.0f, 676.0f, 114.0f, false });
  checkboxBoxT = getTexture("assets/checkbox.png");
  checkboxMarkT = getTexture("assets/checkmark.png");
  checkboxWeatherActiveS = new sf::Sprite(checkboxBoxT);
  checkboxWeatherActiveM = new sf::Sprite(checkboxMarkT);
  checkboxWeatherActive = new Checkbox({ *checkboxWeatherActiveS ,
  *checkboxWeatherActiveM, 639.0f, 258.0f, settings.weatherActive });

  backgroundS->setPosition({ 358.5, 54.5 });
  // Back to Game
  backToGameBtn->setPosition({ 428, 455 });
  backToGameBtn->setFillColor({ 0, 196, 0 });
  backToGameBtn->setStyle(sf::Text::Bold);
  backToGameBtn->setOutlineColor(sf::Color::Black);
  backToGameBtn->setOutlineThickness(1.0);
  // Main Menu
  mainMenuBtn->setFillColor({ 0, 196, 0 });
  mainMenuBtn->setPosition({ 515, 389 });
  mainMenuBtn->setOutlineColor(sf::Color::Black);
  mainMenuBtn->setOutlineThickness(1.0);
  // Restart Level
  restartLevelBtn->setFillColor({ 0, 196, 0 });
  restartLevelBtn->setPosition({ 510, 346 });
  restartLevelBtn->setOutlineColor(sf::Color::Black);
  restartLevelBtn->setOutlineThickness(1.0);
  // Music Slider
  sliderMusic->sprite.setPosition(
    { sliderMusic->lowerBound +
         settings.musicVolume * sliderMusic->length / 100.0f,
     sliderMusic->y });
  // Sound FX Slider
  sliderSFX->sprite.setPosition(
    { sliderSFX->lowerBound +
         settings.soundFXVolume * sliderSFX->length / 100.0f,
     sliderSFX->y });
  // Weather FX Slider
  sliderWeatherFX->sprite.setPosition(
    { sliderWeatherFX->lowerBound +
         settings.weatherFXVolume * sliderWeatherFX->length / 100.0f,
     sliderWeatherFX->y });

  //Weather Active Checkbox
  checkboxWeatherActive->box.setPosition({
    checkboxWeatherActive->x, checkboxWeatherActive->y });
  checkboxWeatherActive->mark.setPosition({
    checkboxWeatherActive->x + 5.0f, checkboxWeatherActive->y - 4.0f });

}

void PauseMenu::update() {

  // Back to Game button
  onClick(*backToGameBtn, []() {
    playSound("ButtonClick");
    setCursorMain();
    gameWeather.isPaused = false, isPaused = false; });

  // Main Menu Button
  onClick(*mainMenuBtn, []() {
    gameState = 0, homeState = 0; //go to home menu not level selector
    playSound("ButtonClick"); playMusic("Menu");
    isPaused = false;
    });

// Restart Level Button
  onClick(*restartLevelBtn, []() {}); // TODO: Add restartLevel()
  

  // Update Sliders
  settings.musicVolume     =  updateSlider(*sliderMusic);
  settings.soundFXVolume   =  updateSlider(*sliderSFX);
  settings.weatherFXVolume =  updateSlider(*sliderWeatherFX);

  //Update Checkboxes
  updateCheckbox(*checkboxWeatherActive, settings.weatherActive);


  updateVolume();

  drawUI();

  draw();
}

void PauseMenu::draw() {

  Sun::manageSuns(0, Sun::State::Paused); // draw only mode
  drawUI();

  window->draw(overlay->overlayRect);
  window->draw(*backgroundS);

  //Buttons
  window->draw(*backToGameBtn);
  window->draw(*mainMenuBtn);
  window->draw(*restartLevelBtn);
  //Sliders
  window->draw(sliderMusic->sprite);
  window->draw(sliderSFX->sprite);
  window->draw(sliderWeatherFX->sprite);
  //Checkboxes & Checkmarks
  window->draw(checkboxWeatherActive->box);
  if (checkboxWeatherActive->checked) window->draw(checkboxWeatherActive->mark);
}