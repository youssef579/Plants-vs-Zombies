#include <newPauseMenu.hpp>
<<<<<<< HEAD
#include <cmath>
=======
#include <UI/Pause.hpp>
#include <LevelManager.hpp>
#include<cmath>
>>>>>>> temp


newPauseMenu newPause;

void newPauseMenu::init() {

  std::string folder = "assets/newPauseMenu/";

  if (!hoverBuffer) hoverBuffer = new sf::SoundBuffer();
  if (hoverBuffer->loadFromFile("assets/sounds/hover.wav")) {
    if (!hoverSound) hoverSound = new sf::Sound(*hoverBuffer);
  }

  if (!clickBuffer) clickBuffer = new sf::SoundBuffer();
  if (clickBuffer->loadFromFile("assets/sounds/tap.wav")) {
    if (!clickSound) clickSound = new sf::Sound(*clickBuffer);
  }

  overlay.setSize({ 2000.f, 2000.f });
  overlay.setFillColor(sf::Color(0, 0, 0, 150));

  float offsetX = 50.0f;
  float offsetY = 20.0f;
  if (!pauseMenu) pauseMenu = new sf::Sprite(getTexture(folder + "newPause (8).png"));
  pauseMenu->setScale({ 0.98,0.98 });
  pauseMenu->setOrigin({ pauseMenu->getLocalBounds().size.x / 2.f, pauseMenu->getLocalBounds().size.y / 2.f });
  pauseMenu->setPosition({ 533.f + offsetX, 300.f + offsetY });

  
  if (!resumeBtn) resumeBtn = new sf::Sprite(getTexture(folder + "resumeButton(4).png"));
  resumeBtn->setScale({ 0.9,0.9 });
  resumeBtn->setOrigin({ resumeBtn->getLocalBounds().size.x / 2.f, resumeBtn->getLocalBounds().size.y / 2.f });
  resumeBtn->setPosition({ 530.5f + offsetX, 290.f + offsetY });

  if (!restartBtn) restartBtn = new sf::Sprite(getTexture(folder + "restartButton(4).png"));
  restartBtn->setScale({ 0.9,0.9 });
  restartBtn->setOrigin({ restartBtn->getLocalBounds().size.x / 2.f, restartBtn->getLocalBounds().size.y / 2.f });
  restartBtn->setPosition({ 529.f + offsetX, 358.f + offsetY });

  if (!settingsBtn) settingsBtn = new sf::Sprite(getTexture(folder + "settingsButton(5).png"));
  settingsBtn->setScale({ 0.8,0.8 });
  settingsBtn->setOrigin({ settingsBtn->getLocalBounds().size.x / 2.f, settingsBtn->getLocalBounds().size.y / 2.f });
  settingsBtn->setPosition({ 531.5f + offsetX, 411.f + offsetY });

  if (!menuBtn) menuBtn = new sf::Sprite(getTexture(folder + "mainMenu(6).png"));
  menuBtn->setOrigin({ menuBtn->getLocalBounds().size.x / 2.f, menuBtn->getLocalBounds().size.y / 2.f });
  menuBtn->setPosition({ 532.5f + offsetX , 479.f + offsetY });


  if (!menuPea) menuPea = new sf::Sprite(getTexture(folder + "peaShooter.png"));
  menuPea->setScale({ 0.4,0.4 });
  menuPea->setOrigin({ menuPea->getLocalBounds().size.x / 2.f, menuPea->getLocalBounds().size.y / 2.f });
  menuPea->setPosition({ 450.f + offsetX, 120.f + offsetY });

  if (!menuSunflower) menuSunflower = new sf::Sprite(getTexture(folder + "sunFlower.png"));
  menuSunflower->setScale({ 0.8,0.8 });


  menuSunflower->setOrigin({ menuSunflower->getLocalBounds().size.x / 2.f, menuSunflower->getLocalBounds().size.y / 2.f });
  menuSunflower->setPosition({ 350.f + offsetX, 300.f + offsetY });

  if (!menuShieldSun) menuShieldSun = new sf::Sprite(getTexture(folder + "shieldSun.png"));
  menuShieldSun->setScale({ 0.3,0.3 });
  menuShieldSun->setOrigin({ menuShieldSun->getLocalBounds().size.x / 2.f, menuShieldSun->getLocalBounds().size.y / 2.f });
  menuShieldSun->setPosition({ 537.f + offsetX, 105.f + offsetY });

  if (!menuWallnut) menuWallnut = new sf::Sprite(getTexture(folder + "wallNut.png"));
  menuWallnut->setScale({ 0.63,0.63 });
  menuWallnut->setOrigin({ menuWallnut->getLocalBounds().size.x / 2.f, menuWallnut->getLocalBounds().size.y / 2.f });
  menuWallnut->setPosition({ 690.0f + offsetX, 360.f + offsetY });


  backgroundOptionsT = getTexture("assets/newPauseMenu/settingsMenu.png");
  if (!backgroundOptionsS) {
    backgroundOptionsS = new  sf::Sprite(backgroundOptionsT);

    backgroundOptionsS->setScale({ 0.5f,0.5f });
    backgroundOptionsS->setPosition({ 325,54.5 });
  }

  if (!backBtn) {
    backBtn = new sf::Sprite(getTexture("assets/newPauseMenu/settingsBackBtn.png"));
    backBtn->setScale({ 0.35f,0.35f });
    backBtn->setOrigin(backBtn->getLocalBounds().getCenter());
    backBtn->setPosition({ 737.0f, 449.0f });
  }

  sliderKnobT = getTexture("assets/newPauseMenu/musicSlider.png");
  if (!sliderMusicS) {
    sliderMusicS = new sf::Sprite(sliderKnobT);
    sliderMusicS->setScale({ 0.3f,0.3f });
    sliderMusicS->setOrigin(sliderMusicS->getLocalBounds().getCenter());
  }
  if (!sliderMusic) sliderMusic = new Slider({ *sliderMusicS,315.0f,550.0f,720.0f,170.0f,false });
  sliderMusic->sprite.setPosition({ sliderMusic->lowerBound + settings.musicVolume * sliderMusic->length / 100.0f,sliderMusic->y });

  if (!sliderSFXS) {
    sliderSFXS = new sf::Sprite(sliderKnobT);
    sliderSFXS->setScale({ 0.3f,0.3f });
    sliderSFXS->setOrigin(sliderSFXS->getLocalBounds().getCenter());
  }
  if (!sliderSFX) sliderSFX = new Slider({ *sliderSFXS,372.0f,550.0f,720.0f,170.0f,false });
  sliderSFX->sprite.setPosition({ sliderSFX->lowerBound + settings.soundFXVolume * sliderSFX->length / 100.0f,sliderSFX->y });

  /*if (!sliderWeatherFXS) sliderWeatherFXS = new sf::Sprite(sliderKnobT);
  if (!sliderWeatherFX) sliderWeatherFX = new Slider({ *sliderWeatherFXS,232.0f,562.0f,676.0f,114.0f,false });
  sliderWeatherFX->sprite.setPosition({ sliderWeatherFX->lowerBound + settings.weatherFXVolume * sliderWeatherFX->length / 100.0f,sliderWeatherFX->y });*/

  checkboxBoxT = getTexture("assets/newPauseMenu/checkerBox.png");
  checkboxMarkT = getTexture("assets/checkmark.png");

  /*if (!checkboxWeatherActiveS) {
    checkboxWeatherActiveS = new sf::Sprite(checkboxBoxT);
    checkboxWeatherActiveS->setScale({ 0.5f,0.5f });
  }
  if (!checkboxWeatherActiveM) {
    checkboxWeatherActiveM = new sf::Sprite(checkboxMarkT);
    checkboxWeatherActiveM->setScale({0.8f,0.8f});
  }
  if (!checkboxWeatherActive) checkboxWeatherActive = new Checkbox({ *checkboxWeatherActiveS,*checkboxWeatherActiveM,639.0f,258.0f,settings.weatherActive });
  checkboxWeatherActive->box.setPosition({ checkboxWeatherActive->x,checkboxWeatherActive->y });
  checkboxWeatherActive->mark.setPosition({ checkboxWeatherActive->x + 10.0f,checkboxWeatherActive->y + 10.0f });*/

  if (!checkboxFullscreenS) {
    checkboxFullscreenS = new sf::Sprite(checkboxBoxT);
    checkboxFullscreenS->setScale({ 0.5f, 0.5f });
  }
  if (!checkboxFullscreenM) {
    checkboxFullscreenM = new sf::Sprite(checkboxMarkT);
    checkboxFullscreenM->setScale({ 0.8f,0.8f });
  }
  if (!checkboxFullscreen) checkboxFullscreen = new Checkbox({ *checkboxFullscreenS,*checkboxFullscreenM,635.0f,400.0f,settings.fullscreen });
  checkboxFullscreen->box.setPosition({ checkboxFullscreen->x,checkboxFullscreen->y });
  checkboxFullscreen->mark.setPosition({ checkboxFullscreen->x + 10.0f,checkboxFullscreen->y + 10.0f });
}


bool newPauseMenu::handleButtonLogic(sf::Sprite* btn, sf::Vector2f mousePos, bool& actionTriggered, bool& isHovered, float baseScale) {
  if (!btn) return false;

<<<<<<< HEAD

=======
  bool prevActionTriggered = actionTriggered;
  //bool prevclicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
>>>>>>> temp

  if (btn->getGlobalBounds().contains(mousePos)) {
    if (!isHovered) {
      if (hoverSound) hoverSound->play();
      isHovered = true;

    }
    if (isMousePressed) {
      if (!actionTriggered) {
        if (clickSound) clickSound->play();
        actionTriggered = true;
      }
      btn->setColor(sf::Color(255, 255, 220));
      btn->setScale({ baseScale * 0.965f,  baseScale * 0.965f });
    }
    else {
      btn->setColor(sf::Color(255, 255, 220));
      btn->setScale({ baseScale * 0.965f,baseScale * 0.965f });
      actionTriggered = false;
    }
  }
  else {
    btn->setColor(sf::Color::White);
    btn->setScale({ baseScale,  baseScale });
    actionTriggered = false;
    isHovered = false;
  }

  if (prevActionTriggered == false && actionTriggered == true) return true;
  return false;
}

void newPauseMenu::update(sf::RenderWindow& window) {
  if (!isOpen) return;

  sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

  if (isSettingsOpen) {
    settings.musicVolume = updateSlider(*sliderMusic);
    settings.soundFXVolume = updateSlider(*sliderSFX);
    //settings.weatherFXVolume = updateSlider(*sliderWeatherFX);

    //updateCheckbox(*checkboxWeatherActive, settings.weatherActive);
    if (updateCheckbox(*checkboxFullscreen, settings.fullscreen)) {
      if (settings.fullscreen) {
        window.create(sf::VideoMode::getDesktopMode(), "Plants vs Zombies", sf::Style::None, sf::State::Fullscreen);
        view->setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        dayLevel.camera.setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        gameView->setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
      }
      else {
        window.create(sf::VideoMode(WINDOW_SIZE), "Plants vs Zombies"); // Default is windowed
        getLetterboxView(window.getSize().x, window.getSize().y);
      }
      setWindowMetaData();
    }

    sounds.updateVolume();

    bool bClicked = false;
    handleButtonLogic(backBtn, mousePos, bClicked, backHovered, 0.35f);

    if (bClicked) isSettingsOpen = false;
  }
  else {
    static bool rClicked = false, resClicked = false, sClicked = false, mClicked = false;


    if (handleButtonLogic(resumeBtn, mousePos, rClicked, resHovered)) {
      isOpen = false;
      isPaused = false;
      gameWeather.isPaused = false;
      setCursorMain();
      if (clickSound) clickSound->play();
    }
    if (handleButtonLogic(menuBtn, mousePos, mClicked, menuHovered)) {
      isOpen = false;
      gameState = 0;
      homeState = 0;
      isPaused = false;

      levelManager.resetLevelData();

      if (clickSound) clickSound->play();
      music.play("Menu");
    }
    if (handleButtonLogic(restartBtn, mousePos, resClicked, restartHovered)) {
      levelManager.resetLevelData();

      isPaused = false;
      gameWeather.isPaused = false;
      isOpen = false;

      if (clickSound) clickSound->play();
    }
    if (handleButtonLogic(settingsBtn, mousePos, sClicked, settingsHovered)) {
      if (clickSound) clickSound->play();
      isSettingsOpen = true;
    }
  }
}

void newPauseMenu::draw(sf::RenderWindow& window) {
  if (!isOpen) return;

  window.draw(overlay);
<<<<<<< HEAD

  if (pauseMenu) window.draw(*pauseMenu);
  if (resumeBtn) window.draw(*resumeBtn);
  if (restartBtn) window.draw(*restartBtn);
  if (settingsBtn) window.draw(*settingsBtn);
  if (menuBtn) window.draw(*menuBtn);
=======
>>>>>>> temp

  if (isSettingsOpen) {
    if (backgroundOptionsS) window.draw(*backgroundOptionsS);
    if (backBtn) window.draw(*backBtn);

    if (sliderMusic) window.draw(sliderMusic->sprite);
    if (sliderSFX) window.draw(sliderSFX->sprite);
    //if (sliderWeatherFX) window.draw(sliderWeatherFX->sprite);

    /*if (checkboxWeatherActive) {
      window.draw(checkboxWeatherActive->box);
      if (checkboxWeatherActive->checked) window.draw(checkboxWeatherActive->mark);
    }*/

    if (checkboxFullscreen) {
      window.draw(checkboxFullscreen->box);
      if (checkboxFullscreen->checked) window.draw(checkboxFullscreen->mark);
    }
  }
  else {
    if (pauseMenu) window.draw(*pauseMenu);
    if (resumeBtn) window.draw(*resumeBtn);
    if (restartBtn) window.draw(*restartBtn);
    if (settingsBtn) window.draw(*settingsBtn);
    if (menuBtn) window.draw(*menuBtn);


    float time = animClock.getElapsedTime().asSeconds();

    if (menuPea) {
      menuPea->setRotation(sf::degrees(std::sin(time * 2.f) * 3.f));
      window.draw(*menuPea);
    }

    if (menuSunflower) {
      float baseScale = 0.8f;
      float anim = 1.0f + std::sin(time * 3.f) * 0.05f;
      menuSunflower->setScale({ baseScale * anim, baseScale * anim });
      window.draw(*menuSunflower);
    }

    if (menuShieldSun) {
      float baseScale = 0.5f;
      float pulse = 1.0f + std::sin(time * 1.5f) * 0.05f;
      menuShieldSun->setScale({ baseScale * pulse, baseScale * pulse });
      window.draw(*menuShieldSun);
    }

    if (menuWallnut) {
      menuWallnut->setRotation(sf::degrees(std::cos(time * 4.f) * 2.f));
      window.draw(*menuWallnut);
    }
  }
}
