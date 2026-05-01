#include <UI/Pause.hpp>
#include <Zombies/Zombie.hpp>
#include <Bullet.hpp>
#include <LevelManager.hpp>
#include <UI/TransitionManager.hpp>

PauseMenu pauseMenu;

PauseMenu::PauseMenu() {}

void PauseMenu::init() {
  //runOnce
  backgroundT = getTexture("assets/pm3.png");
  backgroundS = new sf::Sprite(backgroundT);

  backgroundOptionsT = getTexture("assets/optionsMenu.png");
  backgroundOptionsS = new sf::Sprite(backgroundOptionsT);

  backToGameBtn = new sf::Text(assets->font, "Back To Game", 40);
  mainMenuBtn = new sf::Text(assets->font, "Main Menu", 20);
  restartLevelBtn = new sf::Text(assets->font, "Restart Level", 20);
  doneBtn = new sf::Text(assets->font, "Done", 40);
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

  checkboxFullscreenS = new sf::Sprite(checkboxBoxT);
  checkboxFullscreenM = new sf::Sprite(checkboxMarkT);
  checkboxFullscreen = new Checkbox({ *checkboxFullscreenS ,
  *checkboxFullscreenM, 639.0f, 292.0f, settings.fullscreen });

  backgroundS->setPosition({ 358.5, 54.5 });
  backgroundOptionsS->setPosition({ 358.5, 54.5 });
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
  // Done Button
  doneBtn->setOrigin(doneBtn->getLocalBounds().size / 2.0f);
  doneBtn->setLetterSpacing(2.0f);
  doneBtn->setPosition({ 560, 475 });
  doneBtn->setFillColor({ 0, 196, 0 });
  doneBtn->setStyle(sf::Text::Bold);
  doneBtn->setOutlineColor(sf::Color::Black);
  doneBtn->setOutlineThickness(1.0);
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

  checkboxFullscreen->box.setPosition({
    checkboxFullscreen->x, checkboxFullscreen->y });
  checkboxFullscreen->mark.setPosition({
    checkboxFullscreen->x + 5.0f, checkboxFullscreen->y - 4.0f });

}

void PauseMenu::update() {

  // Other interactions with UI
  shovel.selected = false; // prevents selected carrying over after unpause

  // Back to Game button
  onClick(*backToGameBtn, []() {
    sounds.play("ButtonClick");
    setCursorMain();
    gameWeather.isPaused = false, isPaused = false; });

  // Main Menu Button
  onClick(*mainMenuBtn, []() {
    TransitionManager::start([]() {
        gameState = 0, homeState = 0; //go to home menu not level selector
        isPaused = false;
        gameWeather.isPaused = false;
        levelManager.resetLevelData();
        sounds.play("ButtonClick"); music.play("Menu");
      });
    });

// Restart Level Button
  onClick(*restartLevelBtn, []() {levelManager.restartLevel(); }); // TODO: Add restartLevel()


  // Update Sliders
  settings.musicVolume     =  updateSlider(*sliderMusic);
  settings.soundFXVolume   =  updateSlider(*sliderSFX);
  settings.weatherFXVolume =  updateSlider(*sliderWeatherFX);

  //Update Checkboxes
  updateCheckbox(*checkboxWeatherActive, settings.weatherActive);
  if (updateCheckbox(*checkboxFullscreen, settings.fullscreen)) {
    if (settings.fullscreen) {
      window->create(sf::VideoMode::getDesktopMode(), "Plants vs Zombies", sf::Style::None, sf::State::Fullscreen);
      view->setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
      dayLevel.camera.setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
    }
    else {
      window->create(sf::VideoMode(WINDOW_SIZE), "Plants vs Zombies"); // Default is windowed
      getLetterboxView(window->getSize().x, window->getSize().y);
    }
    setWindowMetaData();
  }


  sounds.updateVolume();

  drawUI();
  // draw plants

  //draw();
}

void PauseMenu::draw() {
  drawGrid();
  Bullet::drawAll();
  Zombie::drawAll();


  drawUI();
  shovel.drawBank();
  drawSeedPackets();
  Sun::drawAll();

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

  window->draw(checkboxFullscreen->box);
  if (checkboxFullscreen->checked) window->draw(checkboxFullscreen->mark);
}



// Options Menu

void PauseMenu::updateOptionsMenu() {

  // Done Button
  onClick(*doneBtn, []() {
      sounds.play("ButtonClick");
      setCursorMain();
      //isPaused = false;
      homeState = 0;
    });

  // Main Menu Button
  //onClick(*mainMenuBtn, []() {
  //  gameState = 0, homeState = 0; //go to home menu not level selector
  //  sounds.play("ButtonClick"); music.play("Menu");
  //  isPaused = false;
  //  });

// Restart Level Button
  //onClick(*restartLevelBtn, []() {}); // TODO: Add restartLevel()


  // Update Sliders
  settings.musicVolume = updateSlider(*sliderMusic);
  settings.soundFXVolume = updateSlider(*sliderSFX);
  settings.weatherFXVolume = updateSlider(*sliderWeatherFX);

  //Update Checkboxes
  updateCheckbox(*checkboxWeatherActive, settings.weatherActive);
  if (updateCheckbox(*checkboxFullscreen, settings.fullscreen)) {
    if (settings.fullscreen)
      window->create(sf::VideoMode::getDesktopMode(), "Plants vs Zombies", sf::Style::None, sf::State::Fullscreen);
    else
      window->create(sf::VideoMode(WINDOW_SIZE), "Plants vs Zombies"); // Default is windowed
    setWindowMetaData();
  }


  sounds.updateVolume();

  //drawUI();
  // draw plants

  drawOptionsMenu();
}

void PauseMenu::drawOptionsMenu() {

  //Sun::manageSuns(0, Sun::State::Paused); // draw only mode
  //drawUI();

  window->draw(overlay->overlayRect);
  window->draw(*backgroundOptionsS);

  //Buttons
  window->draw(*doneBtn);
  //window->draw(*backToGameBtn);
  //window->draw(*mainMenuBtn);
  //window->draw(*restartLevelBtn);
  //Sliders
  window->draw(sliderMusic->sprite);
  window->draw(sliderSFX->sprite);
  window->draw(sliderWeatherFX->sprite);
  //Checkboxes & Checkmarks
  window->draw(checkboxWeatherActive->box);
  if (checkboxWeatherActive->checked) window->draw(checkboxWeatherActive->mark);

  window->draw(checkboxFullscreen->box);
  if (checkboxFullscreen->checked) window->draw(checkboxFullscreen->mark);
}

