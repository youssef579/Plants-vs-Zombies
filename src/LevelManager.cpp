#include <SFML/Graphics.hpp>
#include <LevelManager.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <Window.hpp>
#include <globals.hpp>

LevelManager dayLevel;

void LevelManager::init() {
  
  backGroundTexture = getTexture("assets/BackGround/background.png");
  grassTexture = getTexture("assets/BackGround/sod1row.png");
  threeMiddleGrassTexture = getTexture("assets/BackGround/sod3row.png");
  fullGrassTexture = getTexture("assets/BackGround/fullGrass.png");
  rollTexture = getTexture("assets/BackGround/SodRoll.png");
  capTexture = getTexture("assets/BackGround/SodRollCap.png");
  dirtTexture = getTexture("assets/BackGround/dirtsmall.png");
  
    if (!backGroundSprite) backGroundSprite = new sf::Sprite(backGroundTexture);
    else backGroundSprite->setTexture(backGroundTexture);
    backGroundSprite->setPosition(sf::Vector2f(0, 0));

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
    

      if (!dirtSound) dirtSound = new sf::Sound(getSound("assets/sounds/dirtRoll.wav"));
      dirtSound->setLooping(true);
      dirtSound->setVolume(70.f);

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
  

  view->setSize(sf::Vector2f(800.f, 600.f));
  view->setCenter(sf::Vector2f(490.f, 330.f));
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
  if (isPaused) return; 
  
  if (isIntroRunning) {
    introTimer += dt;

    if (introTimer < 1.5f) view->zoom(1.0f - (0.05 * dt));
    else if (introTimer >= 1.5f && introTimer < 2.2f) view->move(sf::Vector2f(400.f * dt, 0.f));
    else if (introTimer >= 5.5f && introTimer < 6.2f) view->move(sf::Vector2f(-400.f * dt, 0.f));
    else if (introTimer >= 8.5f) {
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


  window.setView(*view);

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

  if (isPaused) {
    sf::View pauseView;
    pauseView.setSize(sf::Vector2f(1066.f, 600.f)); 
    pauseView.setCenter(sf::Vector2f(580.f, 300.f));
    pauseView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
    window.setView(pauseView);
  }
}

void LevelManager::startPlanting() {
  isRolling[2] = true;
  shouldStartRolling[2] = true;
}
