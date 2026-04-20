#include <Array.hpp>
#include <globals.hpp>
#include <SunManager.hpp>
#include <cmath>


Array<Sun> Sun::sunArray;
int Sun::sunBalance = 0;
float Sun::spawnTimer = -20;
bool Sun::hovering = false;

void Sun::manageSuns(float dt, State s) {
  spawnTimer += dt;
  if (spawnTimer >= spawnInterval) { // spawn new sun if needed
    spawnTimer -= spawnInterval;
    Sun::spawn();
  }

  if (s != Paused) {
    for (int i = 0; i < sunArray.size; i++) {
      sunArray[i].update(dt);
      if (sunArray[i].state != Sun::State::Collecting && sunArray[i].onClick())
        hovering = true;
    }

    sunArray.erase([](Sun &sun) {
      return sun.state == Done;
    });

    if (hovering) { setCursorHover(); hovering = false; }
    else setCursorMain();
  }
}

void Sun::update(float dt) {
  //animateSpritesheet(sheet, dt);
  reAnimator.update(dt);
  switch (state) {
    case Falling:
      //sprite.move( { 0, fallSpeed * dt });
      reAnimator.y += fallSpeed * dt;
      if (reAnimator.y >= groundY) // check if sun has hit the ground
        state = OnGround;
      break;

    case FreeFalling:
      // free fall with differnt acc
      //sprite.move( { 0, -(sunFlowerSpeed * dt - 0.5f * acceleration * dt * dt) });
      reAnimator.y -= (sunFlowerSpeed * dt - 0.5f * acceleration * dt * dt);
      sunFlowerSpeed -= acceleration * dt;  // update the speed

      // update scaling as function e ^ x
      if (sunFlowerSpeed >= 0){
        reAnimator.setScale(std::exp(-(1.0f - (sunflowerlevel - reAnimator.y) / distanceSunFlower)), std::exp(-(1.0f - (sunflowerlevel - reAnimator.y) / distanceSunFlower)));
        //sprite.setScale({(float)pow(M_E, -(1.0f - (sunflowerlevel - sprite.getPosition().y) / distanceSunFlower)), (float)pow(M_E, -(1 - (sunflowerlevel - sprite.getPosition().y) / distanceSunFlower))});
      }else{
        if (reAnimator.sx != 1.0f && reAnimator.sy != 1.0f){
          //sprite.setScale({1.0f, 1.0f});
          reAnimator.setScale(1.0f, 1.0f);
        }
      }
      if (reAnimator.y >= sunflowerlevel) // check if sun has hit the sunFlowerLevel
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
      //sprite.move(
        //direction * collectionSpeed * dt / (float)120.0 *
        //distanceToCollection); // Non-linear speed
      reAnimator.move(direction * collectionSpeed * dt / (float)120.0 *
        distanceToCollection);

      distanceToCollection -= collectionSpeed * dt /
        (float)120.0 * distanceToCollection;
      if (distanceToCollection <= collectionErrorMargin) { // Check if Sun has reached
                                                           // collection site
        sunBalance += value;
        state = Done;
      }
      else if (distanceToCollection <= 200.0f) {
        //   let distance = x
        //   while(0 < x < 200) opacity = (x*factor)^2 / 200
        //     where factor = 200 / startX
        //   opacity = x^2 * (factor^2 / 200)                take constant
        //   coefficent as fadeFactor opacity = x^2 * fadeFactor (smooth
        //   transition from opac. = 200 to opac. = 0)
        /*sprite.setColor({ 255, 255, 255,
                                  (uint8_t)(distanceToCollection *
                                            distanceToCollection *
                                            fadeFactor) });*/
        reAnimator.opacityMultiplier = (distanceToCollection *
          distanceToCollection *
          fadeFactor) / 255.0f;

      }

      break;
    }
}

void Sun::generate(sf::Vector2f pos, int val, bool isSunFlower) {
  //static sf::Texture& sunTexture = getTexture("assets/Sun/sun_spritesheet2.png");
  //sf::Sprite sunSprite(sunTexture);
  //sunSprite.setTextureRect({ {0, 0}, {77, 77} }); //set to 1st frame
  Sun* sun;

  if (isSunFlower) {
    sun = new Sun({val, Sun::State::FreeFalling, groundDuration, 0.0, {0.0, 0.0}, 0.0f, float(pos.y), sqrtf(2.0f * acceleration * distanceSunFlower) , ReAnimator(ReAnimator::getDefinition(ReAnimationDef::REANIM_SUN), pos.x, pos.y, window)});
    sun->reAnimator.setScale(0, 0);
  } else
    sun = new Sun({val, Sun::State::Falling, groundDuration, 0.0, {0.0, 0.0}, 0.0f, groundY, fallSpeed, ReAnimator(ReAnimator::getDefinition(ReAnimationDef::REANIM_SUN), pos.x, pos.y, window)});

  //sun->sheet = Spritesheet{ &sun->sprite, 77, 77, 30, 0.03f }; //Initialize spritesheet
  //sun->reAnimator.x = pos.x, sun->reAnimator.y = pos.y;
  sun->reAnimator.setPosition(pos);
  sun->reAnimator.playAnimation("main", true);

  //sun->sprite.setOrigin(sun->sprite.getLocalBounds().size / 2.0f);

  //sun->sprite.setPosition({x, y});

  sunArray.push(*sun);
  delete sun;
}

void Sun::spawn(int val) {
  float randX = randomRange((float)assetWidth, (float)WINDOW_SIZE.x - assetWidth);
  Sun::generate(sf::Vector2f(randX, -70), val, 0);
}

void Sun::draw() {
  reAnimator.draw();
}

void Sun::collect() {
  // Vector from sun to collection site
  direction = collectionSite - reAnimator.getPosition();
  // Get length before normalizing
  distanceToCollection = direction.length();
  // Normalize length to control speed
  direction = direction.normalized();
  if (distanceToCollection >= 200.0f) { // Control fading of sun
    fadeFactor = (1.0f / 200.0f);
    //sprite.setColor({ 255, 255, 255, 200 });
    reAnimator.opacityMultiplier = (200.0f/255.0f);
  }
  else
    fadeFactor = (200.0f / distanceToCollection) *
    (200.0f / distanceToCollection) / 200.0f;
  state = Collecting;
  sounds.play("CollectSun");
}

bool Sun::onClick() {
  static bool hovering = false;

  if (reAnimator.getGlobalBounds().contains(mousePosition)) {
    hovering = true;


    if (isMousePressed) {
      isMousePressed = false; // Avoid clicking many suns at once
      collect();
    }
    return true;
  }
  return false;
}


void Sun::drawAll() {
  for (int i = 0; i < Sun::sunArray.size; i++) {
    Sun::sunArray[i].draw();
  }
}
