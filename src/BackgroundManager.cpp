#include <SFML/Graphics.hpp>
#include <BackgroundManager.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <Window.hpp>
#include <LawnMower.hpp>

LevelManager dayLevel;

void LevelManager::init() {
  
  if (backGroundTexture.loadFromFile("assets/BackGround/background.png") &&
    grassTexture.loadFromFile("assets/BackGround/sod1row.png") &&
    threeMiddleGrassTexture.loadFromFile("assets/BackGround/sod3row.png") &&
    fullGrassTexture.loadFromFile("assets/BackGround/fullGrass1.png") &&
    rollTexture.loadFromFile("assets/BackGround/SodRoll.png") &&
    capTexture.loadFromFile("assets/BackGround/SodRollCap.png"))
  {
    if (!backGroundSprite) backGroundSprite = new sf::Sprite(backGroundTexture);
    else backGroundSprite->setTexture(backGroundTexture);
    backGroundSprite->setPosition(sf::Vector2f(0, 0));

    if (dirtTexture.loadFromFile("assets/BackGround/dirtsmall.png")) {
      dirtPool.clear();
      for (int i = 0; i < 250; i++) {
        DirtParticle p;
        p.sprite = new sf::Sprite(dirtTexture);
        int col = rand() % 8;
        int row = rand() % 2;
        p.sprite->setTextureRect(sf::IntRect({ col * 30, row * 30 }, { 30, 30 }));
        p.sprite->setOrigin({ 15.f, 15.f });

        p.active = false;
        p.lifetime = 0.0f;
        p.velocity = { 0.f, 0.f };
        p.rotationSpeed = 0.f;

        dirtPool.push_back(p);
      }
    }

    if (!dirtBuffer) dirtBuffer = new sf::SoundBuffer();
    if (dirtBuffer->loadFromFile("assets/sounds/dirtRoll.wav")) {
      if (!dirtSound) dirtSound = new sf::Sound(*dirtBuffer);

      dirtSound->setBuffer(*dirtBuffer);
      dirtSound->setLooping(true);
      //dirtSound->setVolume(70.f);
      dirtSound->setVolume(settings.soundFXVolume);
    }

    float startX = 200.0f;
    float rollSize = 0.75f;
    float grassSizeX = 0.82f;
    float grassSizeY = 0.82f;
    targetX = static_cast<float>(grassTexture.getSize().x);
    float bigTargetX = static_cast<float>(threeMiddleGrassTexture.getSize().x);
    for (int i = 0; i < 5; i++) {
      currentX[i] = 0.0f;
      isRolling[i] = false;
      shouldStartRolling[i] = false;
     
      if (i == 0 || i == 2 || i == 4) {
        if (!grassSprites[i]) grassSprites[i] = new sf::Sprite(grassTexture);
        grassSprites[i]->setScale(sf::Vector2f(grassSizeX, grassSizeY));
        grassSprites[i]->setTextureRect(sf::IntRect({ 0, 0 }, { 0, (int)grassTexture.getSize().y }));
        grassSprites[i]->setPosition(sf::Vector2f(startX, groundY[i]));
      }

      if (!rollSprites[i]) rollSprites[i] = new sf::Sprite(rollTexture);
      rollSprites[i]->setOrigin(sf::Vector2f(0.0f, (float)rollTexture.getSize().y / 2.0f));
      rollSprites[i]->setScale(sf::Vector2f(rollSize, rollSize));

      if (!capSprites[i]) capSprites[i] = new sf::Sprite(capTexture);
      capSprites[i]->setOrigin(sf::Vector2f((float)capTexture.getSize().x / 2.0f, (float)capTexture.getSize().y / 2.0f));
      capSprites[i]->setScale(sf::Vector2f(rollSize, rollSize));
    }


    if (!threeMiddleGrassSprite) threeMiddleGrassSprite = new sf::Sprite(threeMiddleGrassTexture);
    threeMiddleGrassSprite->setScale(sf::Vector2f(grassSizeX, grassSizeY));
    float offsetY = -20.0f;
    float offsetX = -8.0f;
    threeMiddleGrassSprite->setPosition(sf::Vector2f(startX + offsetX, groundY[1] + offsetY));
    threeMiddleGrassSprite->setTextureRect(sf::IntRect({ 0, 0 }, { 0, (int)threeMiddleGrassTexture.getSize().y }));

    if (!fullGrassSprite) fullGrassSprite = new sf::Sprite(fullGrassTexture);
    fullGrassSprite->setScale(sf::Vector2f(grassSizeX + 0.01f, grassSizeY + 0.05f)); 
    fullGrassSprite->setPosition(sf::Vector2f(startX - 12.0f, groundY[0] - 48.0f));
    fullGrassSprite->setTextureRect(sf::IntRect({ 0, 0 }, { 0, (int)fullGrassTexture.getSize().y }));
  }

  camera.setSize(sf::Vector2f(800.f, 600.f));
  camera.setCenter(sf::Vector2f(490.f, 312.f));
  gameView->setSize(sf::Vector2f(WINDOW_SIZE.x, WINDOW_SIZE.y));
  //gameView->zoom(0.925f);
  //gameView->setCenter(sf::Vector2f(490.f, 312.f));
  //gameView->setCenter(view->getCenter());
}

void LevelManager::spawnDirt(sf::Vector2f position) {
  int count = 0;
  for (auto& p : dirtPool) {
    if (!p.active) {
      p.active = true;
      
      float offsetX = static_cast<float>(rand() % 40 - 20);
      p.sprite->setPosition(sf::Vector2f(position.x + offsetX, position.y));
     
      float vx = (static_cast<float>(rand() % 400 - 200));
      float vy = -(static_cast<float>(rand() % 250 + 100));
      p.velocity = sf::Vector2f(vx, vy);

      p.lifetime = 0.6f + (static_cast<float>(rand() % 100) / 250.f);
      p.rotationSpeed = static_cast<float>(rand() % 400 - 200);
      p.sprite->setColor(sf::Color::White);

      count++;
      if (count >= 3) break;
    }
  }
}

void LevelManager::updateDirt(float dt) {
  for (auto& p : dirtPool) {
    if (p.active) {
      p.lifetime -= dt;
      p.velocity.y += 800.f * dt; 

      p.sprite->move(p.velocity * dt);
      p.sprite->rotate(sf::degrees(p.rotationSpeed * dt));

      
      if (p.lifetime < 0.3f) {
        sf::Color c = p.sprite->getColor();
        c.a = static_cast<unsigned char>((p.lifetime / 0.3f) * 255);
        p.sprite->setColor(c);
      }

      if (p.lifetime <= 0) p.active = false;
    }
  }
}

void LevelManager::update(float dt) {

  if (isIntroRunning) {
    introTimer += dt;
    float gameViewOffset = 1.5f;
    if (introTimer < 1.5f) {
      //camera.zoom(1.0f - (0.05 * dt));
      //gameView->zoom(1.0f - (0.05 * dt));
      float currentZoom = 1 - (introTimer / 1.5f)*0.075f;
      camera.setSize({800.0f * currentZoom, 600.0f * currentZoom});
    }
    /*else if (introTimer >= 1.5f && introTimer < 2.2f) {
      camera.move(sf::Vector2f(400.f * dt, 0.f));
      gameView->move(sf::Vector2f(400.f * dt * gameViewOffset, 0.f));
    }
    else if (introTimer >= 5.5f && introTimer < 6.2f) {
      camera.move(sf::Vector2f(-400.f * dt, 0.f));
      gameView->move(sf::Vector2f(-400.f * dt * gameViewOffset, 0.f));
    }*/
    else if (introTimer >= 1.5f && introTimer < 2.2f) {
      camera.setCenter({ 490.f + ((introTimer - 1.5f) * 400.f), 312.f });
      gameView->setCenter({ 575.f + ((introTimer - 1.5f) * 600.f), 303.f });
    }
    else if (introTimer >= 5.5f && introTimer < 6.2f) {
      camera.setCenter({ 770.f - ((introTimer - 5.5f) * 400.f), 312.f });
      gameView->setCenter({ 995.f - ((introTimer - 5.5f) * 600.f), 303.f });
    }
    else if (introTimer >= 8.5f) {
      camera.setCenter({ 770.f - ((6.2f - 5.5f) * 400.f), 312.f }); // ensure correct last position
      gameView->setCenter({ 995.f - ((6.2f- 5.5f) * 600.f), 303.f });
      isIntroRunning = false;
      startPlanting();
    }
  }
  if (!isIntroRunning) {
    float grassSizeX = 0.82f;

    if (isRolling[2] && currentX[2] < targetX) {
      currentX[2] += rollSpeed * dt;

      spawnDirt(rollSprites[2]->getPosition());

      int maxTextureW = (int)grassTexture.getSize().x;
      int currentRectW = std::min((int)currentX[2], maxTextureW);

      grassSprites[2]->setTextureRect(sf::IntRect({ 0, 0 }, { currentRectW , (int)grassTexture.getSize().y }));

      float rollSize = 0.7f;
      float finalX = 200.0f + (currentX[2] * grassSizeX);
      float centerY = groundY[2] + (grassTexture.getSize().y * 0.82f) / 2.0f;
      float scaleM = std::max(0.35f, 1.0f - (currentX[2] / targetX));
      float rollW = rollTexture.getSize().x * (scaleM * rollSize);

      rollSprites[2]->setPosition({ finalX - (rollW * 0.1f), centerY - rollSize });
      rollSprites[2]->setScale(sf::Vector2f(scaleM * rollSize, rollSize));

      sf::Vector2f rPos = rollSprites[2]->getPosition();
      capSprites[2]->setPosition(sf::Vector2f(rPos.x + (rollW * 0.5f), rPos.y + (rollTexture.getSize().y * (rollSize / 2.0f))));
      capSprites[2]->setScale(sf::Vector2f(scaleM * rollSize, scaleM * rollSize));
      capSprites[2]->rotate(sf::degrees(400.f * dt * 0.5f));

      if (currentX[2] >= targetX) {
        currentX[2] = targetX;
        isRolling[2] = false;
        isThreeMiddleRunning = true;
      }
    }

    float bigTargetX = static_cast<float>(threeMiddleGrassTexture.getSize().x);

    if (isThreeMiddleRunning && threeMiddleCurrentX < bigTargetX) {
      threeMiddleCurrentX += rollSpeed * dt;
     
      int currentBigRectW = std::min((int)threeMiddleCurrentX, (int)bigTargetX);
      threeMiddleGrassSprite->setTextureRect(sf::IntRect({ 0, 0 }, { currentBigRectW , (int)threeMiddleGrassTexture.getSize().y }));

      for (int j : {1, 3}) {
        spawnDirt(rollSprites[j]->getPosition());
        float finalX = (200.0f - 8.0f) + (threeMiddleCurrentX * grassSizeX);
        float centerY = groundY[j] + (grassTexture.getSize().y * 0.82f) / 2.0f;
        if (j == 3) {
          centerY += 8.0f;
        }
        float scaleM = std::max(0.35f, 1.0f - (threeMiddleCurrentX / bigTargetX));

        float customRollSize = 0.75f;
        float rollW = rollTexture.getSize().x * (scaleM * customRollSize);

        rollSprites[j]->setPosition({ finalX - (rollW * 0.1f), centerY - 6.0f });
        rollSprites[j]->setScale(sf::Vector2f(scaleM * customRollSize, customRollSize));

        sf::Vector2f rPos = rollSprites[j]->getPosition();

        capSprites[j]->setPosition(sf::Vector2f(rPos.x + (rollW * 0.5f), rPos.y + (rollTexture.getSize().y * (customRollSize / 2.0f))));
        capSprites[j]->setScale(sf::Vector2f(scaleM * customRollSize, scaleM * customRollSize));
        capSprites[j]->rotate(sf::degrees(400.f * dt * 0.5f));
      }

      if (threeMiddleCurrentX >= bigTargetX) {
        threeMiddleCurrentX = bigTargetX;
        isThreeMiddleRunning = false;
        isThreeMiddleFinished = true;
        isFullGrassRunning = true;
      }
    }

    float fullTargetX = static_cast<float>(fullGrassTexture.getSize().x);
    if (isFullGrassRunning && fullGrassCurrentX < fullTargetX) {
      fullGrassCurrentX += rollSpeed * dt;
      int currentFullRectW = std::min((int)fullGrassCurrentX, (int)fullTargetX);
      fullGrassSprite->setTextureRect(sf::IntRect({ 0, 0 }, { currentFullRectW , (int)fullGrassTexture.getSize().y }));

      for (int j : {0, 4}) {
        spawnDirt(rollSprites[j]->getPosition());
        float finalX = (200.0f - 12.0f) + (fullGrassCurrentX * (grassSizeX + 0.01));
        float centerY = groundY[j] + (grassTexture.getSize().y * (0.82f + 0.04f)) / 2.0f; 
        if (j == 0) centerY -= 25.0f;
        if (j == 4) centerY += 20.0f;

        float scaleM = std::max(0.35f, 1.0f - (fullGrassCurrentX / fullTargetX));
        float customRollSize = 0.85f;
        float rollW = rollTexture.getSize().x * (scaleM * customRollSize);
        
        rollSprites[j]->setPosition({ finalX - (rollW * 0.05f), centerY });
        rollSprites[j]->setScale(sf::Vector2f(scaleM * customRollSize, customRollSize));
        sf::Vector2f rPos = rollSprites[j]->getPosition();

        float capOffsetY = (rollTexture.getSize().y * customRollSize) * 0.15f;

        if (j == 0) capOffsetY += 40.0f;
        if (j == 4) capOffsetY += 40.0f;
        capSprites[j]->setPosition(sf::Vector2f(rPos.x + (rollW * 0.5f), rPos.y + capOffsetY));
        capSprites[j]->setScale(sf::Vector2f(scaleM * customRollSize, scaleM * customRollSize));
        capSprites[j]->rotate(sf::degrees(400.f * dt * 0.5f));
      }
      if (fullGrassCurrentX >= fullTargetX) {
        fullGrassCurrentX = fullTargetX;
        isFullGrassRunning = false;
        isFullGrassFinished = true;
      }
    }
  }
  bool anyRolling = (isRolling[2] || isThreeMiddleRunning || isFullGrassRunning);
  if (dirtSound) {
    if (anyRolling) {
      dirtSound->setVolume(settings.soundFXVolume); // update volume
      if (!dirtSoundStarted) {
        dirtSound->play();
        dirtSoundStarted = true; 
      }
    }
    else {
      if (dirtSoundStarted) { 
        dirtSound->stop();
        dirtSoundStarted = false; 
      }
    }
  }
  updateDirt(dt);
}


void LevelManager::draw(sf::RenderWindow& window) {

  window.setView(camera);


  if (backGroundSprite) window.draw(*backGroundSprite);


  for (int i : {0, 2, 4}) {
    if (grassSprites[i] && (i != 2 || !isThreeMiddleFinished)) window.draw(*grassSprites[i]);
  }
  if (isThreeMiddleRunning || isThreeMiddleFinished) window.draw(*threeMiddleGrassSprite);

  if (isFullGrassRunning || isFullGrassFinished) window.draw(*fullGrassSprite);

  for (const auto& p : dirtPool) {
    if (p.active && p.sprite) {
      window.draw(*p.sprite);
    }
  }

  for (int i = 0; i < 5; ++i) {
    if (isRolling[i] ||
      (isThreeMiddleRunning && (i == 1 || i == 3)) ||
      (isFullGrassRunning && (i == 0 || i == 4))) {
      window.draw(*rollSprites[i]);
      window.draw(*capSprites[i]);
    }
  }

  //std::cout << "timer: " << introTimer << "\n";
  if (introTimer > 2.5)
    LawnMower::drawAll();

  window.setView(window.getDefaultView());
}

void LevelManager::startPlanting() {
  isRolling[2] = true;
  shouldStartRolling[2] = true;
}
