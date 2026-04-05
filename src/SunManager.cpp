#include <SunManager.hpp>

int Sun::sunBalance = 0;
int Sun::sunArrayCntr = 0;
Sun* Sun::sunArray[Sun::MAX_SIMULTANEOUS_SUN] = { nullptr };
float Sun::spawnTimer = 0;
bool Sun::hovering = false;

void Sun::manageSuns(float dt, State s) {
  spawnTimer += dt;
  if (spawnTimer >= spawnInterval) { // spawn new sun if needed
    spawnTimer -= spawnInterval;
    Sun::spawn();
  }

  if (s != Paused) {
    for (int i = 0; i < sunArrayCntr; i++) {
      if (!sunArray[i]) continue;
      if (!sunArray[i]->update(dt)) { i--; continue; }
      if (sunArray[i]->state != Sun::State::Collecting && sunArray[i]->onClick()) hovering = true;
      sunArray[i]->draw();
    }

    if (hovering) { setCursorHover(); hovering = false; }
    else setCursorMain();

  }
  else
    for (int i = 0; i < sunArrayCntr; i++) {
      if (!sunArray[i]) continue;
      sunArray[i]->draw();
    }
}

bool Sun::update(float dt) {
  animateSpritesheet(sheet, dt);
  switch (state) {
    case Falling:
    
      sprite.move( { 0, fallSpeed * dt });
      if (sprite.getPosition().y >= groundY) // check if sun has hit the ground
        state = OnGround; 
      break;

    case OnGround:
      groundTimer -= dt;
      if (groundTimer <= 0) // auto-collect sun
        collect();
      else if (onClick())
        hovering = true;
      
      break;

    case Collecting:
      sprite.move(
        direction * collectionSpeed * dt / (float)120.0 *
        distanceToCollection); // Non-linear speed
      
      distanceToCollection -= collectionSpeed * dt /
        (float)120.0 * distanceToCollection;
      if (distanceToCollection <= collectionErrorMargin) { // Check if Sun has reached
                                                           // collection site
        sunBalance += value;
        Sun::destroy(index);
        return false;
      }
      else if (distanceToCollection <= 200.0f) {
        //   let distance = x
        //   while(0 < x < 200) opacity = (x*factor)^2 / 200
        //     where factor = 200 / startX
        //   opacity = x^2 * (factor^2 / 200)                take constant
        //   coefficent as fadeFactor opacity = x^2 * fadeFactor (smooth
        //   transition from opac. = 200 to opac. = 0)
        sprite.setColor({ 255, 255, 255,
                                  (uint8_t)(distanceToCollection *
                                            distanceToCollection *
                                            fadeFactor) });
      }

      break;
    }


  return true; // Object still alive
}

void Sun::generate(float x, float y, int val) {
  static sf::Texture& sunTexture = getTexture("assets/Sun/sun_spritesheet2.png");
  sf::Sprite sunSprite(sunTexture);
  sunSprite.setTextureRect({ {0, 0}, {77, 77} }); //set to 1st frame
  Sun* sun =
    new Sun({ sunSprite, val, Sun::State::Falling, groundDuration,
      0.0, {0.0, 0.0}, 0.0f, sunArrayCntr, nullptr });
  sun->sheet = Spritesheet{ &sun->sprite, 77, 77, 30, 0.03f }; //Initialize spritesheet

  sun->sprite.setPosition({ x, y });

  sunArray[sunArrayCntr] = sun;
  sunArrayCntr++;

}

void Sun::spawn(int val) {
  float randX =
        randomRange((float)assetWidth, (float)WINDOW_SIZE.x - assetWidth);
  Sun::generate(randX, -70, val);
}

void Sun::draw() {
  window->draw(sprite);
}

void Sun::collect() {
  // Vector from sun to collection site
  direction = collectionSite - sprite.getPosition();
  // Get length before normalizing
  distanceToCollection = direction.length();
  // Normalize length to control speed
  direction = direction.normalized();
  if (distanceToCollection >= 200.0f) { // Control fading of sun
    fadeFactor = (1.0f / 200.0f);
    sprite.setColor({ 255, 255, 255, 200 });
  }
  else
    fadeFactor = (200.0f / distanceToCollection) *
    (200.0f / distanceToCollection) / 200.0f;
  state = Collecting;
  playSound("CollectSun");
}

bool Sun::onClick() {
  static bool wasButtonClicked = false;
  static bool hovering = false;
  sf::Vector2f mousePosition =
    window->mapPixelToCoords(sf::Mouse::getPosition(*window));
  
  if (sprite.getGlobalBounds().contains(mousePosition)) {
    hovering = true;
  
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!wasButtonClicked) {
        collect();
        wasButtonClicked = true;
      }
    }
    else
      wasButtonClicked = false;
  
    return true;
  }
  return false;
}

void Sun::destroy(int idx) {
  //Todo: Replace with dynamic array
  delete sunArray[idx];
  sunArray[idx] = nullptr;
  for (int i = idx; i < sunArrayCntr-1; i++) {
    sunArray[i] = sunArray[i + 1];
    sunArray[i]->index--;
  }
  sunArray[sunArrayCntr - 1] = nullptr;

  sunArrayCntr--;
}