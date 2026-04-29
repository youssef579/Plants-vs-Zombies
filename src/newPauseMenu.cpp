#include <newPauseMenu.hpp>


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
  pauseMenu->setPosition({533.f + offsetX, 300.f + offsetY});


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

}


void newPauseMenu::handleButtonLogic(sf::Sprite* btn, sf::Vector2f mousePos, bool& actionTriggered , bool& isHovered) {
  if (!btn) return;

  

  if (btn->getGlobalBounds().contains(mousePos)) {
    if (!isHovered) {
      if (hoverSound) hoverSound->play();
      isHovered = true;

    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!actionTriggered) {
        if (clickSound) clickSound->play();
        actionTriggered = true;
      }
      btn->setColor(sf::Color(255, 255, 220));
      btn->setScale({ 0.965f,  0.965f });
    }
    else {
      btn->setColor(sf::Color(255, 255, 220));
      btn->setScale({ 0.965f, 0.965f });
      actionTriggered = false;
    }
  }
  else {
    btn->setColor(sf::Color::White);
    btn->setScale({ 0.98f,  0.98f });
    actionTriggered = false;
    isHovered = false; 
  }
}

void newPauseMenu::update(sf::RenderWindow& window) {
  if (!isOpen) return;

  sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

  bool rClicked = false, resClicked = false, sClicked = false, mClicked = false;

  handleButtonLogic(resumeBtn, mousePos, rClicked, resHovered);
  handleButtonLogic(restartBtn, mousePos, resClicked, restartHovered);
  handleButtonLogic(settingsBtn, mousePos, sClicked, settingsHovered);
  handleButtonLogic(menuBtn, mousePos, mClicked, menuHovered);

  if (rClicked) isOpen = false;
}

void newPauseMenu::draw(sf::RenderWindow& window) {
  if (!isOpen) return;

  window.draw(overlay);
  
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
