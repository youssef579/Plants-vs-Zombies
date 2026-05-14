#include "globals.hpp"
#include <LevelManager.hpp>
#include <UI/Pause.hpp>
#include <cmath>
#include <newPauseMenu.hpp>
#include <UI/TransitionManager.hpp>

newPauseMenu newPause;


/* =====================================================================
 * FUNCTION: newPauseMenu::init()
 * ---------------------------------------------------------------------
 * Initializes the entire Pause and Settings menu. It loads the necessary
 * audio buffers for hover and click sounds. It sets up the dark transparent
 * overlay, loads textures for all buttons, sliders, checkboxes, and
 * decorative plant sprites, and centers their origins and
 * positions them on the screen with specific offsets.
 * ===================================================================== */
void newPauseMenu::init() {

  std::string folder = "assets/newPauseMenu/";

  if (!hoverBuffer)
    hoverBuffer = new sf::SoundBuffer();
  if (hoverBuffer->loadFromFile("assets/sounds/hover.wav")) {
    if (!hoverSound)
      hoverSound = new sf::Sound(*hoverBuffer);
  }

  if (!clickBuffer)
    clickBuffer = new sf::SoundBuffer();
  if (clickBuffer->loadFromFile("assets/sounds/tap.wav")) {
    if (!clickSound)
      clickSound = new sf::Sound(*clickBuffer);
  }

  overlay.setSize({2000.f, 2000.f});                      // for bluring behind the pause menu 
  overlay.setFillColor(sf::Color(0, 0, 0, 150));         //

  float offsetX = 50.0f;
  float offsetY = 20.0f;
  if (!pauseMenu)
    pauseMenu = new sf::Sprite(getTexture(folder + "newPause (8).png"));
  pauseMenu->setScale({0.98, 0.98});
  pauseMenu->setOrigin({pauseMenu->getLocalBounds().size.x / 2.f,
                        pauseMenu->getLocalBounds().size.y / 2.f});
  pauseMenu->setPosition({533.f + offsetX, 300.f + offsetY});

  if (!resumeBtn)
    resumeBtn = new sf::Sprite(getTexture(folder + "resumeButton(4).png"));
  resumeBtn->setScale({0.9, 0.9});
  resumeBtn->setOrigin({resumeBtn->getLocalBounds().size.x / 2.f,
                        resumeBtn->getLocalBounds().size.y / 2.f});
  resumeBtn->setPosition({530.5f + offsetX, 290.f + offsetY});

  if (!restartBtn)
    restartBtn = new sf::Sprite(getTexture(folder + "restartButton(4).png"));
  restartBtn->setScale({0.9, 0.9});
  restartBtn->setOrigin({restartBtn->getLocalBounds().size.x / 2.f,
                         restartBtn->getLocalBounds().size.y / 2.f});
  restartBtn->setPosition({529.f + offsetX, 358.f + offsetY});

  if (!settingsBtn)
    settingsBtn = new sf::Sprite(getTexture(folder + "settingsButton(5).png"));
  settingsBtn->setScale({0.8, 0.8});
  settingsBtn->setOrigin({settingsBtn->getLocalBounds().size.x / 2.f,
                          settingsBtn->getLocalBounds().size.y / 2.f});
  settingsBtn->setPosition({531.5f + offsetX, 411.f + offsetY});

  if (!menuBtn)
    menuBtn = new sf::Sprite(getTexture(folder + "mainMenu(6).png"));
  menuBtn->setOrigin({menuBtn->getLocalBounds().size.x / 2.f,
                      menuBtn->getLocalBounds().size.y / 2.f});
  menuBtn->setPosition({532.5f + offsetX, 479.f + offsetY});

  if (!menuPea)
    menuPea = new sf::Sprite(getTexture(folder + "peaShooter.png"));
  menuPea->setScale({0.4, 0.4});
  menuPea->setOrigin({menuPea->getLocalBounds().size.x / 2.f,
                      menuPea->getLocalBounds().size.y / 2.f});
  menuPea->setPosition({450.f + offsetX, 120.f + offsetY});

  if (!menuSunflower)
    menuSunflower = new sf::Sprite(getTexture(folder + "sunFlower.png"));
  menuSunflower->setScale({0.8, 0.8});

  menuSunflower->setOrigin({menuSunflower->getLocalBounds().size.x / 2.f,
                            menuSunflower->getLocalBounds().size.y / 2.f});
  menuSunflower->setPosition({350.f + offsetX, 300.f + offsetY});

  if (!menuShieldSun)
    menuShieldSun = new sf::Sprite(getTexture(folder + "shieldSun.png"));
  menuShieldSun->setScale({0.3, 0.3});
  menuShieldSun->setOrigin({menuShieldSun->getLocalBounds().size.x / 2.f,
                            menuShieldSun->getLocalBounds().size.y / 2.f});
  menuShieldSun->setPosition({537.f + offsetX, 105.f + offsetY});

  if (!menuWallnut)
    menuWallnut = new sf::Sprite(getTexture(folder + "wallNut.png"));
  menuWallnut->setScale({0.63, 0.63});
  menuWallnut->setOrigin({menuWallnut->getLocalBounds().size.x / 2.f,
                          menuWallnut->getLocalBounds().size.y / 2.f});
  menuWallnut->setPosition({690.0f + offsetX, 360.f + offsetY});

  backgroundOptionsT = getTexture("assets/newPauseMenu/settingsMenu.png");
  if (!backgroundOptionsS) {
    backgroundOptionsS = new sf::Sprite(backgroundOptionsT);

    backgroundOptionsS->setScale({0.5f, 0.5f});
    backgroundOptionsS->setPosition({325, 54.5});
  }

  //initialize Back Button
  if (!backBtn) {
    backBtn =
        new sf::Sprite(getTexture("assets/newPauseMenu/settingsBackBtn.png"));
    backBtn->setScale({0.35f, 0.35f});
    backBtn->setOrigin(backBtn->getLocalBounds().getCenter());
    backBtn->setPosition({737.0f, 449.0f});
  }

  //intialize Slider
  sliderKnobT = getTexture("assets/newPauseMenu/musicSlider2.png");

  //initialize Music Slider
  if (!sliderMusicS) {
    sliderMusicS = new sf::Sprite(sliderKnobT);
    sliderMusicS->setScale({0.3f, 0.3f});
    sliderMusicS->setOrigin(sliderMusicS->getLocalBounds().getCenter());
  }
  if (!sliderMusic)
    sliderMusic = new Slider({*sliderMusicS, 307.0f, 550.0f, 720.0f, 170.0f, false}); //Settings Slider
  sliderMusic->sprite.setPosition(
      {sliderMusic->lowerBound + settings.musicVolume * sliderMusic->length / 100.0f, sliderMusic->y}); //slider Location

  //initialize Sound Slider
  if (!sliderSFXS) {
    sliderSFXS = new sf::Sprite(sliderKnobT);
    sliderSFXS->setScale({0.3f, 0.3f});
    sliderSFXS->setOrigin(sliderSFXS->getLocalBounds().getCenter());
  }
  if (!sliderSFX)
    sliderSFX = new Slider({*sliderSFXS, 372.0f, 550.0f, 720.0f, 170.0f, false}); //Settings Slider
  sliderSFX->sprite.setPosition(
    {sliderSFX->lowerBound + settings.soundFXVolume * sliderSFX->length / 100.0f, sliderSFX->y}); //slider Location

  //initialize Checker Box
  checkboxBoxT = getTexture("assets/newPauseMenu/checkerBox.png");
  //initialize Checker Box Marker
  checkboxMarkT = getTexture("assets/checkmark.png");

  //initialize Full Screen Box
  if (!checkboxFullscreenS) {
    checkboxFullscreenS = new sf::Sprite(checkboxBoxT);
    checkboxFullscreenS->setScale({0.5f, 0.5f});
  }
  //initialize Full Screen Marker
  if (!checkboxFullscreenM) {
    checkboxFullscreenM = new sf::Sprite(checkboxMarkT);
    checkboxFullscreenM->setScale({0.8f, 0.8f});
  }
  if (!checkboxFullscreen)
    checkboxFullscreen =
        new Checkbox({*checkboxFullscreenS, *checkboxFullscreenM, 635.0f, 400.0f, settings.fullscreen}); //Check Box Settings
  checkboxFullscreen->box.setPosition( {checkboxFullscreen->x, checkboxFullscreen->y});
  checkboxFullscreen->mark.setPosition( {checkboxFullscreen->x + 10.0f, checkboxFullscreen->y + 10.0f});
}



/* =====================================================================
 * FUNCTION: newPauseMenu::handleButtonLogic()
 * ---------------------------------------------------------------------
 * A highly reusable  function that handles all button states.
 * It checks if the mouse is hovering over the button, applies a darker
 * tint and plays a sound if hovered. It shrinks the button when clicked,
 * and finally returns true only when the mouse is released over the button,
 * ensuring a complete and proper "click" action.
 * ===================================================================== */
bool newPauseMenu::handleButtonLogic(sf::Sprite *btn, sf::Vector2f mousePos,
                                     bool &actionTriggered, bool &isHovered,
                                     float baseScale) {
  if (!btn)
    return false;

  bool prevActionTriggered = actionTriggered;

  if (btn->getGlobalBounds().contains(mousePos)) {
    if (!isHovered) {
      if (hoverSound)
        hoverSound->play();
      isHovered = true;
    }
    if (isMousePressed) {
      if (!actionTriggered) {
        if (clickSound)
          clickSound->play();
        actionTriggered = true;
      }
      btn->setColor(sf::Color(255, 255, 220));
      btn->setScale({baseScale * 0.965f, baseScale * 0.965f});
    } else {
      btn->setColor(sf::Color(255, 255, 220));
      btn->setScale({baseScale * 0.965f, baseScale * 0.965f});
      actionTriggered = false;
    }
  } else {
    btn->setColor(sf::Color::White);
    btn->setScale({baseScale, baseScale});
    actionTriggered = false;
    isHovered = false;
  }

  if (prevActionTriggered == false && actionTriggered == true)
    return true;
  return false;
}



/* =====================================================================
 * FUNCTION: newPauseMenu::update()
 * ---------------------------------------------------------------------
 * The main logic loop for the menu. Maps mouse coordinates. If the settings
 * menu is open, it handles slider dragging, fullscreen toggles (re-creating
 * the window if needed), and the back button. If the main pause menu is open,
 * it processes clicks on the Resume, Restart, Settings, and Main Menu
 * buttons, handling game state transitions accordingly.
 * ===================================================================== */
void newPauseMenu::update(sf::RenderWindow &window) {
  if (!isOpen)
    return;

  sf::Vector2f mousePos =
      window.mapPixelToCoords(sf::Mouse::getPosition(window));

  if (isSettingsOpen) {
    //Update Sliders
    settings.musicVolume = updateSlider(*sliderMusic);
    settings.soundFXVolume = updateSlider(*sliderSFX);

    //Update CheckerBoxes
    if (updateCheckbox(*checkboxFullscreen, settings.fullscreen)) {
      if (settings.fullscreen) {
        window.create(sf::VideoMode::getDesktopMode(), "Plants vs Zombies", sf::Style::None, sf::State::Fullscreen);
        view->setViewport(sf::FloatRect({0.0f, 0.0f}, {1.0f, 1.0f}));
        dayLevel.camera.setViewport(sf::FloatRect({0.0f, 0.0f}, {1.0f, 1.0f}));
        gameView->setViewport(sf::FloatRect({0.0f, 0.0f}, {1.0f, 1.0f}));
      } else {
        window.create(sf::VideoMode(WINDOW_SIZE),
                      "Plants vs Zombies"); // Default is windowed
        getLetterboxView(window.getSize().x, window.getSize().y);
      }
      setWindowMetaData();
    }

    sounds.updateVolume();

    // Update Back Button
    bool bClicked = false;
    handleButtonLogic(backBtn, mousePos, bClicked, backHovered, 0.35f);

    if (bClicked)
      isSettingsOpen = false;
  } else {
    static bool rClicked = false, resClicked = false, sClicked = false, mClicked = false;

    //Update Resume 
    if (handleButtonLogic(resumeBtn, mousePos, rClicked, resHovered)) {
      isOpen = isPaused = gameWeather.isPaused = false;
    }
    //Update Return Menu
    if (handleButtonLogic(menuBtn, mousePos, mClicked, menuHovered)) {
      setCursorMain();
      TransitionManager::start([&]() {
      isOpen = false;
        gameState = 0, homeState = 0; //go to home menu not level selector
        isPaused = false;
        gameWeather.isPaused = false;
        levelManager.resetLevelData();
        sounds.play("ButtonClick"); music.play("Menu");
        });

      if (clickSound)
        clickSound->play();
    }
    //Update Restart Level
    if (handleButtonLogic(restartBtn, mousePos, resClicked, restartHovered)) {
      setCursorMain();
      TransitionManager::start([&]() {
        isOpen = false;
        levelManager.restartLevel();
        });

      if (clickSound)
        clickSound->play();
    }
    //Update Settings
    if (handleButtonLogic(settingsBtn, mousePos, sClicked, settingsHovered)) {
      if (clickSound)
        clickSound->play();
      isSettingsOpen = true;
    }
  }
}

void newPauseMenu::draw(sf::RenderWindow &window) {
  if (!isOpen)
    return;

  window.draw(overlay);
  if (pauseMenu)
    window.draw(*pauseMenu);
  if (resumeBtn)
    window.draw(*resumeBtn);
  if (restartBtn)
    window.draw(*restartBtn);
  if (settingsBtn)
    window.draw(*settingsBtn);
  if (menuBtn)
    window.draw(*menuBtn);

  if (isSettingsOpen) {
    if (backgroundOptionsS)
      window.draw(*backgroundOptionsS);
    if (backBtn)
      window.draw(*backBtn);

    if (sliderMusic)
      window.draw(sliderMusic->sprite);
    if (sliderSFX)
      window.draw(sliderSFX->sprite);

    if (checkboxFullscreen) {
      window.draw(checkboxFullscreen->box);
      if (checkboxFullscreen->checked)
        window.draw(checkboxFullscreen->mark);
    }
  } else {
    if (pauseMenu)
      window.draw(*pauseMenu);
    if (resumeBtn)
      window.draw(*resumeBtn);
    if (restartBtn)
      window.draw(*restartBtn);
    if (settingsBtn)
      window.draw(*settingsBtn);
    if (menuBtn)
      window.draw(*menuBtn);

    float time = animClock.getElapsedTime().asSeconds();               // for anim 

    if (menuPea) {                                                     // right and left 3deg
      menuPea->setRotation(sf::degrees(std::sin(time * 2.f) * 3.f));   
      window.draw(*menuPea);
    }

    if (menuSunflower) {
      float baseScale = 0.8f;
      float anim = 1.0f + std::sin(time * 3.f) * 0.05f;
      menuSunflower->setScale({baseScale * anim, baseScale * anim});
      window.draw(*menuSunflower);
    }

    if (menuShieldSun) {
      float baseScale = 0.5f;
      float pulse = 1.0f + std::sin(time * 1.5f) * 0.05f;
      menuShieldSun->setScale({baseScale * pulse, baseScale * pulse});
      window.draw(*menuShieldSun);
    }

    if (menuWallnut) {
      menuWallnut->setRotation(sf::degrees(std::cos(time * 4.f) * 2.f));
      window.draw(*menuWallnut);
    }
  }
}
