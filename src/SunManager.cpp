#include <SunManager.hpp>

int SunBalance = 0;
int sunArrayCounter = 0;
float sunSpawnTimer = 0.0;
const int MAX_SUN_SIZE = 250;
Sun* SunArray[MAX_SUN_SIZE] = {
    nullptr }; // bi size 250 bta5od 76kb memory bs fa looks good
const float SUN_FALL_SPEED = 60.0f;
const float SUN_GROUND_HEIGHT = 450.0f; // y level that sun stops at
const float SUN_GROUND_TIMER =
3.0f; // time the sun stays on the ground before auto-collect (in seconds)
const sf::Vector2f SUN_COLLECTION_SITE = {
    0.0f, 0.0f }; // where collected sun will go when clicked
const float SUN_COLLECTION_SPEED = 300.0f;
const float SUN_COLLECTION_ERROR_MARGIN =
35.0f; // Distance from which sun will be considered collected
const float SUN_ASSET_SIZE = 77;      // needed for spawn bounds
const float SUN_SPAWN_INTERVAL = 1.0; // time between each sun spawn (in
// seconds)


void manageSuns(float dt) {
  sunSpawnTimer += dt;
  if (sunSpawnTimer >= SUN_SPAWN_INTERVAL) { // spawn new sun if needed
    sunSpawnTimer -= SUN_SPAWN_INTERVAL;
    spawnSun();
  }
  updateSun(dt);
  drawSun();
}

void drawSun() {
  for (auto& currSun : SunArray) {
    if (currSun) {
      window->draw(currSun->sprite);
    }
  }
}

void updateSun(float dt) {
  static bool hovering = false, runOnce = true;

  for (auto& currSun : SunArray) {
    if (currSun) {
      animateSpritesheet(currSun->sheet, dt);
      //if (!isPaused)   NOTE: msh lazm if(!isPaused) 3shan kda kda updateSun() mbt7slsh lw paused
        //animateFrames(currSun->sprite, frames, nFrames, currSun->currentFrame,
                      //currSun->frameTimer);
      switch (currSun->state) {
      case 0: // sun is falling

        currSun->sprite.move(
          { 0,
           SUN_FALL_SPEED *
               dt }); // same thing as
        // currSun->setPosition(currSun->getPosition() + offset)
        if (currSun->sprite.getPosition().y >= SUN_GROUND_HEIGHT)
          currSun->state = 1; // check if sun has hit the ground
        if (onClickSun(currSun, collectSun))
          hovering = true;
        break;

      case 1: // sun is on ground
        currSun->groundTimer -= dt;
        if (currSun->groundTimer <= 0) { // auto-collect sun
          collectSun(currSun);
          break;
        }
        if (onClickSun(currSun, collectSun))
          hovering = true;
        break;

      case 2: // sun is moving to collection site
        // habadt equation lel motion w sob7analah tl3t shabah el aslya faaa
        currSun->sprite.move(
          currSun->direction * SUN_COLLECTION_SPEED * dt / (float)120.0 *
          currSun->distFromCollectionSite); // curved(?) speed
        currSun->distFromCollectionSite -= SUN_COLLECTION_SPEED * dt /
          (float)120.0 *
          currSun->distFromCollectionSite;
        if (currSun->distFromCollectionSite <=
          SUN_COLLECTION_ERROR_MARGIN) { // Check if Sun has reached
          // collection site
          SunBalance += currSun->value;
          delete currSun;
          currSun = nullptr;
        }
        else if (currSun->distFromCollectionSite <= 200.0f) {
          // Second habda of the month:
          //   let distance = x
          //   while(0 < x < 200) opacity = (x*factor)^2 / 200
          //     where factor = 200 / startX
          //   opacity = x^2 * (factor^2 / 200)                take constant
          //   coefficent as fadeFactor opacity = x^2 * fadeFactor (smooth
          //   transition from opac. = 200 to opac. = 0)
          currSun->sprite.setColor({ 255, 255, 255,
                                    (uint8_t)(currSun->distFromCollectionSite *
                                              currSun->distFromCollectionSite *
                                              currSun->fadeFactor) });
        }

        break;
      }
    }
  }

  // Handle switching of cursor images when hovering (or not)
  if (hovering) {
    setCursorHover();
    hovering = false;
  }
  else
    setCursorMain();
}

void generateSun(float x, float y, int value) {
  //static sf::Texture &sunTexture = getTexture("assets/Sun/Sun_0.png");
  static sf::Texture& sunTexture = getTexture("assets/Sun/sun_spritesheet2.png");
  sf::Sprite sunSprite(sunTexture);
  sunSprite.setTextureRect({ {0, 0}, {77, 77} }); //set to 1st frame
  Sun* sun =
    new Sun({ sunSprite, value, 0, SUN_GROUND_TIMER, 0.0, {0.0, 0.0}, 0.0f, nullptr });
  sun->sheet = Spritesheet{ &sun->sprite, 77, 77, 30, 0.03f }; //Initialize spritesheet

  sun->sprite.setPosition({ x, y });

  SunArray[sunArrayCounter] = sun;
  sunArrayCounter++;

}

void spawnSun(int value) {
  float randX =
    randomRange(SUN_ASSET_SIZE, window->getSize().x - SUN_ASSET_SIZE);
  generateSun(randX, -70, value);
}

bool onClickSun(Sun*& sun, std::function<void(Sun* s)> action) {
  static bool wasButtonClicked = false;
  static bool hovering = false;
  sf::Vector2f mousePosition =
    window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  if (sun->sprite.getGlobalBounds().contains(mousePosition)) {
    hovering = true;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!wasButtonClicked) {
        action(sun);
        wasButtonClicked = true;
      }
    }
    else
      wasButtonClicked = false;

    return true;
  }
  return false;
}

void collectSun(Sun* sun) {
  sun->direction =
    SUN_COLLECTION_SITE -
    sun->sprite.getPosition(); // Vector from sun to collection site
  sun->distFromCollectionSite =
    sun->direction.length(); // get length before normalizing
  sun->direction =
    sun->direction.normalized(); // normalize length to control speed
  if (sun->distFromCollectionSite >= 200.0f) { // Control fading of sun
    sun->fadeFactor = (1.0f / 200.0f);
    sun->sprite.setColor({ 255, 255, 255, 200 });
  }
  else
    sun->fadeFactor = (200.0f / sun->distFromCollectionSite) *
    (200.0f / sun->distFromCollectionSite) / 200.0f;
  sun->state = 2;
  playSound("CollectSun");
}