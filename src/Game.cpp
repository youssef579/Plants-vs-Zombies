#include <Game.hpp>
#include <Home.hpp>
#include <Overlay.hpp>
#include <AssetsManager.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <random> //lw la2eto tari2a mn8er library feel free bs di azon standard library asln
int gameState = 0;
/*
  0 -> Home menu
*/
bool isPaused = false;

//  a8lb el const mmkn yt3mlo hard-coded b3d ma netefe2 3la el values bs ma7toten
//  ka placeholder 3shan yb2o more accessible
int SunBalance = 0;
int sunArrayCounter = 0;
float sunSpawnTimer = 0.0;
const int MAX_SUN_SIZE = 250;
Sun* SunArray[MAX_SUN_SIZE] = { nullptr }; //bi size 250 bta5od 76kb memory bs fa looks good
const float SUN_FALL_SPEED = 50.0;
const float SUN_GROUND_HEIGHT = 400.0; //y level that sun stops at
const float SUN_GROUND_TIMER = 3.0; //time the sun stays on the ground before auto-collect (in seconds)
const sf::Vector2f SUN_COLLECTION_SITE = { 30.0, 30.0 }; //where collected sun will go when clicked
const float SUN_COLLECTION_SPEED = 300.0;
const float SUN_COLLECTION_ERROR_MARGIN = 15.0; //Distance from which sun will be considered collected
const float SUN_ASSET_SIZE = 77; //needed for spawn bounds
const float SUN_SPAWN_INTERVAL = 1.0; //time between each sun spawn (in seconds)

sf::Clock drawClock;
float dt; //Delta Time (time between each frame draw)


bool runOnce = true;
bool runOncePause = true;
void updateGame() {
  dt = drawClock.restart().asSeconds(); //clock.restart() sets time to 0 and returns the last time before modifying it
  //calling dt = clock.restart() each frame returns the time between frames (dt)



  switch (gameState) {
  case 0:
    updateHome();
    break;
  default:
    //Replace with loadLevel() w copy kol el sun logic hnak
    if (isPaused) { updatePause(); break; }

    if (runOnce) { //will be used later (and drawUI() uses runOnce)
      runOnce = false;
    }
    manageSuns();

    drawUI();
    break;
  }
}

void manageSuns() {
  sunSpawnTimer += dt;
  if (sunSpawnTimer >= SUN_SPAWN_INTERVAL) { //spawn new sun if needed
    sunSpawnTimer -= SUN_SPAWN_INTERVAL;
    spawnSun();
  }
  updateSun();
  drawSun();
}

void drawSun() {
  for (auto& currSun : SunArray) {
    if (currSun) {
      window->draw(currSun->sprite);
    }
  }

}

void updateSun() {
  static bool hovering = false;
  for (auto& currSun : SunArray) {
    if (currSun) {
      switch (currSun->state) {
      case 0: //sun is falling
        currSun->sprite.move({ 0, SUN_FALL_SPEED * dt }); //same thing as currSun->setPosition(currSun->getPosition() + offset)
        if (currSun->sprite.getPosition().y >= SUN_GROUND_HEIGHT) currSun->state = 1; //check if sun has hit the ground
        if (onClickSun(currSun, collectSun)) hovering = true;
        break;

      case 1: //sun is on ground
        currSun->groundTimer -= dt;
        if (currSun->groundTimer <= 0) { //auto-collect sun
          collectSun(currSun);
          break;
        }
        if (onClickSun(currSun, collectSun)) hovering = true;
        break;

      case 2: //sun is moving to collection site
        //habadt equation lel motion w sob7analah tl3t shabah el aslya faaa
        currSun->sprite.move(currSun->direction * SUN_COLLECTION_SPEED * dt / (float)120.0 * currSun->distFromCollectionSite);   //curved(?) speed
        currSun->distFromCollectionSite -= SUN_COLLECTION_SPEED * dt / (float)120.0 * currSun->distFromCollectionSite;
        if (currSun->distFromCollectionSite <= SUN_COLLECTION_ERROR_MARGIN) { //Check if Sun has reached collection site
          SunBalance += currSun->value;
          delete currSun;
          currSun = nullptr;
        }
        break;


      }


    }
  }

  //Handle switching of cursor images when hovering (or not)
  if (hovering) {
    setCursorHover();
    hovering = false;
  }
  else
    setCursorMain();

}

void generateSun(float x, float y, int value) {
  static sf::Texture& sunTexture = getTexture("assets/sun.png");
  sf::Sprite sunSprite(sunTexture);
  Sun* sun = new Sun({ sunSprite, value, 0, SUN_GROUND_TIMER, 0.0, {0.0, 0.0} });

  sun->sprite.setPosition({ x, y });

  SunArray[sunArrayCounter] = sun;
  sunArrayCounter++;

  return;
}

void spawnSun(int value) {
  //random number using normal distribution (asef 3la ma2asy el probability ya shbab)
  //probably h8yrha b3den li 7aga simpler
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(SUN_ASSET_SIZE, window->getSize().x - SUN_ASSET_SIZE); //bounds to spawn sun in

  generateSun(dist(gen), -70, value);

}

void collectSun(Sun* sun) {
  sun->direction = SUN_COLLECTION_SITE - sun->sprite.getPosition(); //Vector from sun to collection site
  sun->distFromCollectionSite = sun->direction.length(); //get length before normalizing
  sun->direction = sun->direction.normalized(); //normalize length to control speed
  sun->state = 2;
}



void updatePause() {
  static sf::Texture& PauseMenuTexture = getTexture("assets/pause-menu-4.png");
  static sf::Sprite PauseMenuSprite(PauseMenuTexture);


  //Back to Game button
  static sf::Text PauseMenuBacktoGameButton(assets->font, "Back To Game", 40);
  onClick(PauseMenuBacktoGameButton, []() {isPaused = false; });

  //Main Menu Button
  static sf::Text PauseMenuMainMenuButton(assets->font, "Main Menu", 20);
  onClick(PauseMenuMainMenuButton, []() {gameState = 0; isPaused = false; }); //TODO: Handle resetting level data (currently aknk 2flt w rg3t btkml mkank)

  //Restart Level Button
  static sf::Text PauseMenuRestartLevelButton(assets->font, "Restart Level", 20);
  onClick(PauseMenuRestartLevelButton, []() {}); //TODO: Add restartLevel()


  if (runOncePause) {
    PauseMenuSprite.setPosition({ 358.5 , 54.5 });
    //Back to Game
    PauseMenuBacktoGameButton.setPosition({ 428 , 455 });
    PauseMenuBacktoGameButton.setFillColor({ 0, 196, 0 });
    PauseMenuBacktoGameButton.setStyle(sf::Text::Bold);
    PauseMenuBacktoGameButton.setOutlineColor(sf::Color::Black);
    PauseMenuBacktoGameButton.setOutlineThickness(1.0);
    //Main Menu
    PauseMenuMainMenuButton.setFillColor({ 0, 196, 0 });
    PauseMenuMainMenuButton.setPosition({ 515 , 389 });
    PauseMenuMainMenuButton.setOutlineColor(sf::Color::Black);
    PauseMenuMainMenuButton.setOutlineThickness(1.0);
    //Restart Level
    PauseMenuRestartLevelButton.setFillColor({ 0, 196, 0 });
    PauseMenuRestartLevelButton.setPosition({ 510 , 346 });
    PauseMenuRestartLevelButton.setOutlineColor(sf::Color::Black);
    PauseMenuRestartLevelButton.setOutlineThickness(1.0);

    runOncePause = false;
  }


  drawUI();
  drawSun();

  window->draw(PauseMenuSprite);
  window->draw(PauseMenuBacktoGameButton);
  window->draw(PauseMenuMainMenuButton);
  window->draw(PauseMenuRestartLevelButton);

}


void drawUI() {
  static sf::Text SunBalanceText(assets->font, std::to_string(SunBalance), 40);
  SunBalanceText.setString(std::to_string(SunBalance));
  if (runOnce) SunBalanceText.setPosition({ 20.0, 20.0 });
  window->draw(SunBalanceText);

}
