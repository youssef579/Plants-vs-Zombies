#include <SFML/Graphics.hpp>
#include <BackgroundManager.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <Window.hpp>
#include <LawnMower.hpp>
#include <Game.hpp>
#include <UI/TransitionManager.hpp>
#include <LevelManager.hpp>


BackgroundManager dayLevel;

/* =====================================================================
 * FUNCTION: BackgroundManager::init
 * ---------------------------------------------------------------------
 * Responsible for loading all necessary graphical and audio assets
 * (textures, sounds) from the disk into memory. It sets up the initial
 * camera views, initializes the object pool for dirt particles, and
 * prepares the sprites for the sod-rolling animations depending on the
 * level type (Day/Night).
 * ===================================================================== */
void BackgroundManager::init(LevelManager::Level::Location Type) {
  type = Type;
  if (backGroundTexture.loadFromFile("assets/Background/background.png") &&
    grassTexture.loadFromFile("assets/Background/sod1row.png") &&
    threeMiddleGrassTexture.loadFromFile("assets/Background/sod3row.png") &&
    fullGrassTexture.loadFromFile("assets/Background/fullGrass1.png") &&
    rollTexture.loadFromFile("assets/Background/SodRoll.png") &&
    capTexture.loadFromFile("assets/Background/SodRollCap.png") &&
    zombiesWonTexture.loadFromFile("assets/Background/zombiesWon.png"))
  {
    if (type == LevelManager::Level::Night) backGroundTexture = getTexture("assets/Background/background_night.png");

    if (!backGroundSprite) backGroundSprite = new sf::Sprite(backGroundTexture);
    else backGroundSprite->setTexture(backGroundTexture);
    backGroundSprite->setPosition(sf::Vector2f(0, 0));

    if (!zombiesWon) zombiesWon = new sf::Sprite(zombiesWonTexture);
    zombiesWon->setScale({ 0.5, 0.5 });

    if (dirtTexture.loadFromFile("assets/Background/dirtsmall.png")) {
      camera.setSize(sf::Vector2f(800.f, 600.f));
      camera.setCenter(sf::Vector2f(490.f, 312.f));
      gameView->setSize(sf::Vector2f(WINDOW_SIZE.x, WINDOW_SIZE.y));

      if (type == LevelManager::Level::Night) return;

      if (dirtTexture.loadFromFile("assets/Background/dirtsmall.png")) {
        dirtPool.clear();
        for (int i = 0; i < 250; i++) {
          DirtParticle p;
          p.sprite = new sf::Sprite(dirtTexture);
          int col = rand() % 8;
          int row = rand() % 2;
          p.sprite->setTextureRect(sf::IntRect({ col * 30, row * 30 }, { 30, 30 }));         // 30*30 pixels from large
          p.sprite->setOrigin({ 15.f, 15.f });                                               // 30/2 --> rotate around itself

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
        dirtSound->setVolume(settings.soundFXVolume);
      }


      // Pre-calculate target widths and starting positions for sod rolling
      float startX = 200.0f;
      float rollSize = 0.75f;
      float grassSizeX = 0.82f;
      float grassSizeY = 0.82f;
      targetX = static_cast<float>(grassTexture.getSize().x);
      float bigTargetX = static_cast<float>(threeMiddleGrassTexture.getSize().x);

      // Setup the 5 individual rows of grass and their rolling carpets
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
      threeMiddleGrassSprite->setTextureRect(sf::IntRect({ 0, 0 }, { 0, (int)threeMiddleGrassTexture.getSize().y }));  // disappearing the grass at first

      if (!fullGrassSprite) fullGrassSprite = new sf::Sprite(fullGrassTexture);
      fullGrassSprite->setScale(sf::Vector2f(grassSizeX + 0.01f, grassSizeY + 0.05f));
      fullGrassSprite->setPosition(sf::Vector2f(startX - 12.0f, groundY[0] - 48.0f));
      fullGrassSprite->setTextureRect(sf::IntRect({ 0, 0 }, { 0, (int)fullGrassTexture.getSize().y }));
    }
  }
}


/* =====================================================================
 * FUNCTION: BackgroundManager::spawnDirt
 * ---------------------------------------------------------------------
 * Finds inactive dirt particles in the pre-allocated pool, activates
 * them at the specified position, and assigns them random velocities,
 * lifetimes, and rotation speeds to simulate debris flying out.
 * ===================================================================== */
void BackgroundManager::spawnDirt(sf::Vector2f position) {
  int count = 0;                           // counter for number of particles in this call
  for (auto& p : dirtPool) {
    if (!p.active) {
      p.active = true;

      // Generate a random horizontal offset between -20 and +19 to make the dirt spawn in a scattered area
      float offsetX = static_cast<float>(rand() % 40 - 20);
      p.sprite->setPosition(sf::Vector2f(position.x + offsetX, position.y));

      // Generate a random horizontal velocity (X-axis) between -200 (left) and +199 (right)
      float vx = (static_cast<float>(rand() % 400 - 200));

      // Generate a random vertical velocity (Y-axis) between -100 and -349. Note: Negative Y means moving UPWARDS in SFML
      float vy = -(static_cast<float>(rand() % 250 + 100));

      p.velocity = sf::Vector2f(vx, vy);

      // Randomize how long the particle stays on screen (between 0.6 and 0.99 seconds)
      p.lifetime = 0.6f + (static_cast<float>(rand() % 100) / 250.f);

      // Randomize the rotation speed and direction (negative for counter-clockwise, positive for clockwise) to rotate about itself while flying
      p.rotationSpeed = static_cast<float>(rand() % 400 - 200);
      p.sprite->setColor(sf::Color::White);

      count++;
      if (count >= 3) break;
    }
  }
}



/* =====================================================================
 * FUNCTION: BackgroundManager::updateDirt
 * ---------------------------------------------------------------------
 * Iterates through all active dirt particles every frame, applying
 * physics (gravity, rotation) and fading them out (adjusting alpha)
 * as they approach the end of their lifetime.
 * ===================================================================== */
void BackgroundManager::updateDirt(float dt) {
  for (auto& p : dirtPool) {
    if (p.active) {
      p.lifetime -= dt;            // Decrease remaining lifetime
      p.velocity.y += 800.f * dt;  // when y->(negative)->(the particle is flying) therefore I added (800*dt) (gravity)

      p.sprite->move(p.velocity * dt);
      p.sprite->rotate(sf::degrees(p.rotationSpeed * dt));


      if (p.lifetime < 0.3f) {
        sf::Color c = p.sprite->getColor();          // current dirt particle color
        c.a = static_cast<unsigned char>((p.lifetime / 0.3f) * 255);  // a(alpha(transparency degree [0]-->[255]))
        p.sprite->setColor(c);
      }

      if (p.lifetime <= 0) p.active = false;
    }
  }
}


/* =====================================================================
 * FUNCTIONS: rollOneGrass, rollThreeGrass, rollFiveGrass
 * ---------------------------------------------------------------------
 * These functions control the animation of the grass/sod
 * being unrolled at the start of a day level. They expand the grass
 * texture's visible rectangle, move/shrink the cylinder sprite, rotate
 * the cap sprite, and trigger dirt spawning until the grass is fully laid.
 * ===================================================================== */
void BackgroundManager::rollOneGrass(float dt) {
  float grassSizeX = 0.82f;

  if (isRolling[2] && currentX[2] < targetX) {
    currentX[2] += rollSpeed * dt;

    spawnDirt(rollSprites[2]->getPosition());

    int maxTextureW = (int)grassTexture.getSize().x;
    int currentRectW = std::min((int)currentX[2], maxTextureW);
    grassSprites[2]->setTextureRect(sf::IntRect({ 0, 0 }, { currentRectW , (int)grassTexture.getSize().y }));

    float rollSize = 0.7f;
    float finalX = 200.0f + (currentX[2] * grassSizeX);        // roll final pos to be compatible with grass
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
}

void BackgroundManager::rollThreeGrass(float dt) {

  float grassSizeX = 0.82f;
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
}

void BackgroundManager::rollFiveGrass(float dt) {
  float grassSizeX = 0.82f;
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





void BackgroundManager::update(float dt) {

  if (state == GameOver) {
    updateGameOverScreen(dt);
    return;
  }

  if (iceFlashTimer > 0) {
    iceFlashTimer -= dt;
    iceFlashTimer = std::max(0.0f, iceFlashTimer);
  }

  if (!isWaitingForPlay) introTimer += dt;


  if (isIntroRunning) {

    if (introTimer < 1.5f) { //zoomm in
      float currentZoom = 1 - (std::max(introTimer, 0.0f) / 1.5f)*0.075f; // allows negative values for delays
      camera.setSize({800.0f * currentZoom, 600.0f * currentZoom});
    }
    else if (introTimer >= 1.5f && introTimer < 2.2f) { // right
      camera.setSize({800.0f * 0.925f, 600.0f * 0.925f});
      camera.setCenter({ 490.f + ((introTimer - 1.5f) * 400.f), 312.f });
      gameView->setCenter({ 575.f + ((introTimer - 1.5f) * 600.f), 303.f });


      if (introTimer >= 2.15f && !introSlidingStarted) {
        plantSelector.slideIn();
        introSlidingStarted = true;
        isWaitingForPlay = true;
      }

    }

    plantSelector.updateSelector(dt, *window);

    if (introTimer >= 5.5f && introTimer < 6.2f) { // left
      camera.setCenter({ 770.f - ((std::min(introTimer,6.15f) - 5.5f) * 400.f), 312.f});
      gameView->setCenter({ 995.f - ((std::min(introTimer,6.15f) - 5.5f) * 600.f), 303.f });

    }
    else if (introTimer >= 8.5f) {
      camera.setCenter({ 770.f - ((std::min(introTimer,6.15f) - 5.5f) * 400.f), 312.f});
      gameView->setCenter({ 995.f - ((std::min(introTimer,6.15f) - 5.5f) * 600.f), 303.f });

      isIntroRunning = false;

      if (type == LevelManager::Level::Night) {
        if(introTimer <=20.f)
          introTimer = 20.0f;
        return;
      }
      startPlanting();
    }
  }

rollOneGrass(dt);
rollThreeGrass(dt);
rollFiveGrass(dt);

bool anyRolling = (isRolling[2] || isThreeMiddleRunning || isFullGrassRunning);
if (dirtSound) {
  if (anyRolling) {
    dirtSound->setVolume(settings.soundFXVolume * 0.45f);

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


void BackgroundManager::draw(sf::RenderWindow& window) {

  window.setView(camera);


  if (backGroundSprite) window.draw(*backGroundSprite);

  if(levelManager.levels[levelManager.currentLevel - 1]->location == LevelManager::Level::Day) {

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
  }

  if (introTimer > 2.5)
    LawnMower::drawAll();


  window.setView(window.getDefaultView());
}

void BackgroundManager::drawOverlays(sf::RenderWindow &window) {
  if (state == GameOver && gameOverTimer >= 2) {
    window.setView(camera);
    window.draw(overlay->overlayRect);
    window.draw(*zombiesWon);
    window.setView(window.getDefaultView());
    return;
  }

  static sf::RectangleShape iceFlash = []() {
      sf::RectangleShape rec((sf::Vector2f)WINDOW_SIZE);
      return rec;
    }();
  if (iceFlashTimer > 0) {
    iceFlash.setFillColor(sf::Color(100, 200, 255, (uint8_t)ReAnimator::lerp(0, 200, iceFlashTimer/iceFlashDuration) ));
    window.draw(iceFlash);
  }



}

void BackgroundManager::startPlanting() {
  isRolling[2] = true;
  shouldStartRolling[2] = true;

  dirtSoundStarted = false;

  // Kill Dummy Zombies
  for (int r = 0; r < ROWS_NUMBER; r++) zombies[r].erase([](Zombie *z) { return true; });

}


void BackgroundManager::playGameOverScreen(int dr) {
  if (state != GameOver) {
    state = GameOver;
    gameOverTimer = 0;
    globalTimeModifier = 0.1;
    deathRow = dr;
    music.stop();
    sounds.play("LoseMusic");
  }
}

void BackgroundManager::updateGameOverScreen(float dt) {
  //std::cout << "grassPos: " << dayLevel.fullGrassSprite->getPosition().x << " " << dayLevel.fullGrassSprite->getPosition().y << "\n";
  gameOverTimer += dt / globalTimeModifier;
  zombiesWonTimer += dt / globalTimeModifier;

  //static sf::Texture zombiesWonT = getTexture("assets/Background/ZombiesWon.png");
  //static sf::Sprite zombiesWon(zombiesWonT);

  if (gameOverTimer > 2.0f) {
    gameOverTimer = 2.0f;
  }

  float lerpF = gameOverTimer / 2.0f;
  // wlahi ma AI ana b7eb el math zyada bs ~_~
      //lerpF = (lerpF * lerpF) * (2 - 1 * lerpF);
  // ax^3 + bx^2 + cx + d
  // a = -0.408333333333333, b = -0.275, c = 1.683333, d = 0
  // a = -49/120, b = -11/40, c = 101/60, d = 0
  lerpF = (-49.0f/120.0f) * lerpF * lerpF * lerpF + (-11.0f/40.0f) * lerpF * lerpF + (101.0f/60.0f) * lerpF + 0.0f;




  //static sf::Vector2f targetPos = { 100.0f + 100.0f, 400.0f };
  static sf::Vector2f startCenterCamera;
  static sf::Vector2f startSizeCamera;
  static sf::Vector2f startCenterGameView;
  static sf::Vector2f startSizeGameView;
  static sf::Vector2f startCenterView;
  static sf::Vector2f startSizeView;

  static sf::Vector2f endCenterCamera;
  static sf::Vector2f endSizeCamera;
  static sf::Vector2f endCenterGameView;
  static sf::Vector2f endSizeGameView;
  static sf::Vector2f endCenterView;
  static sf::Vector2f endSizeView;

  //static float duration = 5.0f;
  //static float timer = 0.0f;
  static bool capturedStart = false;

  if (!capturedStart) {
    startCenterCamera = camera.getCenter();
    startSizeCamera = camera.getSize();

    startCenterGameView = gameView->getCenter();
    startSizeGameView = gameView->getSize();

    startCenterView = view->getCenter();
    startSizeView = view->getSize();

    sf::Vector2f targetPos = grid[deathRow][0].rectangle.getGlobalBounds().getCenter() + sf::Vector2f(20.f, 0.f);
    if(deathRow == ROWS_NUMBER - 1) targetPos -= sf::Vector2f(0.0f, 80.f);

    sf::Vector2f cameraPanDistance = targetPos - startCenterCamera;

    float gameViewRatioX = startSizeGameView.x / startSizeCamera.x;
    float gameViewRatioY = startSizeGameView.y / startSizeCamera.y;

    float viewRatioX = startSizeView.x / startSizeCamera.x;
    float viewRatioY = startSizeView.y / startSizeCamera.y;

    endCenterCamera = targetPos;

    endCenterGameView = {
        startCenterGameView.x + (cameraPanDistance.x * gameViewRatioX),
        startCenterGameView.y + (cameraPanDistance.y * gameViewRatioY)
    };

    endCenterView = {
        startCenterView.x + (cameraPanDistance.x * viewRatioX),
        startCenterView.y + (cameraPanDistance.y * viewRatioY)
    };

    float sizeMulti = 0.5f;
    endSizeCamera = startSizeCamera * sizeMulti;
    endSizeGameView = startSizeGameView * sizeMulti;
    endSizeView = startSizeView * sizeMulti;

    capturedStart = true;
  }


  /*timer += dt / globalTimeModifier;
  timer = std::min(timer, duration);
  float t = timer / duration;*/

  camera.setSize(startSizeCamera + (endSizeCamera - startSizeCamera) * lerpF);
  gameView->setSize(startSizeGameView + (endSizeGameView - startSizeGameView) * lerpF);
  view->setSize(startSizeView + (endSizeView - startSizeView) * lerpF);

  camera.setCenter(startCenterCamera + (endCenterCamera - startCenterCamera) * lerpF);
  gameView->setCenter(startCenterGameView + (endCenterGameView - startCenterGameView) * lerpF);
  view->setCenter(startCenterView + (endCenterView - startCenterView) * lerpF);




  zombiesWon->setOrigin(zombiesWon->getLocalBounds().size / 2.0f);
  zombiesWon->setPosition(camera.getCenter());
  if (zombiesWonTimer >= 2) {
    //if (zombiesWonTimer > 2 + 0.5) zombiesWonTimer = 2 + 0.5;
    sf::Color oldColor = zombiesWon->getColor();
    zombiesWon->setColor({oldColor.r, oldColor.g, oldColor.b, (uint8_t)(0 + (255)*std::min(zombiesWonTimer-2, 0.5f)/0.5f)  });
  }
  if (zombiesWonTimer >= 6) {
    static bool startedTransition = false;
    if (!startedTransition) {
      startedTransition = true;
      TransitionManager::start([&]() {
        view->setCenter(startCenterView);
        view->setSize(startSizeView);
        gameView->setCenter(startCenterGameView);
        gameView->setSize(startSizeGameView);
        camera.setCenter(startCenterCamera);
        camera.setSize(startSizeCamera);
        capturedStart = false;
        gameState = 0;
        homeState = 0;
        music.play("Menu");
        });
    }

  }

}


void testKeybinds(std::string key) {
  const float x = 5;
  const float sF = 0.001f;

  // Background sprite
  //BG_Pos(620, 300)          620, 300
  //BG_Size(1.575, 1.07)      1.545, 1.095

  //Full Grass--------------------------------------------
    //FG_Pos(528, 367)          FG_Pos(522, 363)
    //FG_Size(1.295, 0.925)     FG_Size(1.27, 0.895)

  // Three middle
    //FG_Pos(532, 358)
    //FG_Size(1.27, 0.835)



  static bool state = [](){

    dayLevel.camera.setSize((sf::Vector2f)WINDOW_SIZE * 0.875f);
    return false;
  }();

  if (key == "sw")
    state = !state;

  if (!state) {
    if (key == "j")
      dayLevel.backGroundSprite->move(sf::Vector2f{ -x, 0 });
    if (key == "k")
      dayLevel.backGroundSprite->move(sf::Vector2f{ x, 0 });
    if (key == "i")
      dayLevel.backGroundSprite->move(sf::Vector2f{ 0, -x });
    if (key == "m")
      dayLevel.backGroundSprite->move(sf::Vector2f{ 0, x });

    if (key == "up")
      dayLevel.backGroundSprite->setScale(dayLevel.backGroundSprite->getScale() + sf::Vector2f{ 0, x * sF });
    if (key == "down")
      dayLevel.backGroundSprite->setScale(dayLevel.backGroundSprite->getScale() + sf::Vector2f{ 0, -x * sF });
    if (key == "right")
      dayLevel.backGroundSprite->setScale(dayLevel.backGroundSprite->getScale() + sf::Vector2f{ x * sF, 0 });
    if (key == "left")
      dayLevel.backGroundSprite->setScale(dayLevel.backGroundSprite->getScale() + sf::Vector2f{ -x * sF, 0 });

    std::cout << "BG--------------------------------------------\n";
    std::cout << "BG_Pos (" << dayLevel.backGroundSprite->getPosition().x << ", " << dayLevel.backGroundSprite->getPosition().y << ")\n";
    std::cout << "BG_Size(" << dayLevel.backGroundSprite->getScale().x << ", " << dayLevel.backGroundSprite->getScale().y << ")\n";
    return;
  }


  //Grass
  //for (int i = 0; i < 5; i++) {
    sf::Sprite *sp = dayLevel.fullGrassSprite;
    if (key == "j")
      sp->move(sf::Vector2f{ -x, 0 });
    if (key == "k")
      sp->move(sf::Vector2f{ x, 0 });
    if (key == "i")
      sp->move(sf::Vector2f{ 0, -x });
    if (key == "m")
      sp->move(sf::Vector2f{ 0, x });


    if (key == "up")
      sp->setScale(sp->getScale() + sf::Vector2f{ 0, x * sF });
    if (key == "down")
      sp->setScale(sp->getScale() + sf::Vector2f{ 0, -x * sF });
    if (key == "right")
      sp->setScale(sp->getScale() + sf::Vector2f{ x * sF, 0 });
    if (key == "left")
      sp->setScale(sp->getScale() + sf::Vector2f{ -x * sF, 0 });

    std::cout << "Full Grass--------------------------------------------\n";
    std::cout << "FG_Pos (" << sp->getPosition().x << ", " << sp->getPosition().y << ")\n";
    std::cout << "FG_Size(" << sp->getScale().x << ", " << sp->getScale().y << ")\n";
    return;

}
