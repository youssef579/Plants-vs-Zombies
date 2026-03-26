#include <Game.hpp>
#include <Home.hpp>
#include <Overlay.hpp>
#include <AssetsManager.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <Audio.hpp>
int gameState = 0;
/*
  0 -> Home menu
*/

//  a8lb el const mmkn yt3mlo hard-coded b3d ma netefe2 3la el values bs ma7toten
//  ka placeholder 3shan yb2o more accessible
int SunBalance = 0;
int sunArrayCounter = 0;
float sunSpawnTimer = 0.0;
const int MAX_SUN_SIZE = 250;
Sun* SunArray[MAX_SUN_SIZE] = { nullptr }; //bi size 250 bta5od 76kb memory bs fa looks good
const float SUN_FALL_SPEED = 60.0f;
const float SUN_GROUND_HEIGHT = 450.0f; //y level that sun stops at
const float SUN_GROUND_TIMER = 3.0f; //time the sun stays on the ground before auto-collect (in seconds)
const sf::Vector2f SUN_COLLECTION_SITE = { 30.0f, 30.0f }; //where collected sun will go when clicked
const float SUN_COLLECTION_SPEED = 300.0f;
const float SUN_COLLECTION_ERROR_MARGIN = 35.0f; //Distance from which sun will be considered collected
const float SUN_ASSET_SIZE = 77; //needed for spawn bounds
const float SUN_SPAWN_INTERVAL = 1.0; //time between each sun spawn (in seconds)

sf::Clock drawClock;
float dt; //Delta Time (time between each frame draw)

bool isPaused = false;
bool runOnce = true;
bool runOncePause = true;
bool runOnceUI = true;
void updateGame() {
  dt = drawClock.restart().asSeconds(); //clock.restart() sets time to 0 and returns the last time before modifying it
  //calling dt = clock.restart() each frame returns the time between frames (dt)


  switch (gameState) {
  case 0:
    updateHome();
    break;
  default:
    //Replace with loadLevel() w copy kol el logic hnak
    if (isPaused) { updatePause(); break; }

    if (runOnce) {
      playMusic("DayStage");
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
        else if (currSun->distFromCollectionSite <= 200.0f) {
          //Second habda of the month:
          //  let distance = x
          //  while(0 < x < 200) opacity = (x*factor)^2 / 200
          //    where factor = 200 / startX
          //  opacity = x^2 * (factor^2 / 200)                take constant coefficent as fadeFactor
          //  opacity = x^2 * fadeFactor           (smooth transition from opac. = 200 to opac. = 0)
          currSun->sprite.setColor({ 255, 255, 255, (uint8_t)(currSun->distFromCollectionSite * currSun->distFromCollectionSite * currSun->fadeFactor) });
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
  Sun* sun = new Sun({ sunSprite, value, 0, SUN_GROUND_TIMER, 0.0, {0.0, 0.0} , 0.0f });

  sun->sprite.setPosition({ x, y });

  SunArray[sunArrayCounter] = sun;
  sunArrayCounter++;

  return;
}

void spawnSun(int value) {
  float randX = randomRange(SUN_ASSET_SIZE, window->getSize().x - SUN_ASSET_SIZE);
  generateSun(randX, -70, value);
}

void collectSun(Sun* sun) {
  sun->direction = SUN_COLLECTION_SITE - sun->sprite.getPosition(); //Vector from sun to collection site
  sun->distFromCollectionSite = sun->direction.length(); //get length before normalizing
  sun->direction = sun->direction.normalized(); //normalize length to control speed
  if (sun->distFromCollectionSite >= 200.0f) { //Control fading of sun
    sun->fadeFactor = (1.0f / 200.0f);
    sun->sprite.setColor({255, 255, 255, 200});
  }
  else
    sun->fadeFactor = (200.0f / sun->distFromCollectionSite) * (200.0f / sun->distFromCollectionSite) / 200.0f;
  sun->state = 2;
  playSound("CollectSun");
}



void updatePause() {
  static sf::Texture& PauseMenuTexture = getTexture("assets/pause-menu.png");
  static sf::Sprite PauseMenuSprite(PauseMenuTexture);


  //Back to Game button
  static sf::Text PauseMenuBacktoGameButton(assets->font, "Back To Game", 40);
  onClick(PauseMenuBacktoGameButton, []() {isPaused = false; });

  //Main Menu Button
  static sf::Text PauseMenuMainMenuButton(assets->font, "Main Menu", 20);
  onClick(PauseMenuMainMenuButton, []() {gameState = 0; playMusic("Menu"); isPaused = false; }); //TODO: Handle resetting level data (currently aknk 2flt w rg3t btkml mkank)

  //Restart Level Button
  static sf::Text PauseMenuRestartLevelButton(assets->font, "Restart Level", 20);
  onClick(PauseMenuRestartLevelButton, []() {}); //TODO: Add restartLevel()

  static sf::Texture& PauseMenuSliderTexture = getTexture("assets/slider.png");
  //Music Slider
  static sf::Sprite PauseMenuMusicSliderSprite(PauseMenuSliderTexture);
  static Slider PauseMenuMusicSlider = { PauseMenuMusicSliderSprite,
    178.0f, 566.0f, 676.0f, 110.0f, false };
  //SoundFX Slider
  static sf::Sprite PauseMenuSoundFXSliderSprite(PauseMenuSliderTexture);
  static Slider PauseMenuSoundFXSlider = { PauseMenuSoundFXSliderSprite,
    205.0f, 566.0f, 676.0f, 110.0f, false };


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
    //Music Slider
    PauseMenuMusicSlider.sprite.setPosition({PauseMenuMusicSlider.lowerBound + Settings_MusicVolume*PauseMenuMusicSlider.length/100.0f, PauseMenuMusicSlider.y}); //Min Bound
    //Sound FX Slider
    PauseMenuSoundFXSlider.sprite.setPosition({PauseMenuSoundFXSlider.lowerBound + Settings_SoundFXVolume* PauseMenuSoundFXSlider.length/100.0f, PauseMenuSoundFXSlider.y}); //Min Bound
    


    runOncePause = false;
  }

  Settings_MusicVolume = updateSlider(PauseMenuMusicSlider);
  Settings_SoundFXVolume = updateSlider(PauseMenuSoundFXSlider);

  updateVolume(Settings_MusicVolume, Settings_SoundFXVolume);

  drawUI();
  drawSun();

  window->draw(PauseMenuSprite);
  window->draw(PauseMenuBacktoGameButton);
  window->draw(PauseMenuMainMenuButton);
  window->draw(PauseMenuRestartLevelButton);
  window->draw(PauseMenuMusicSlider.sprite);
  window->draw(PauseMenuSoundFXSlider.sprite);

}

void drawUI() {
  static sf::Text SunBalanceText(assets->font, std::to_string(SunBalance), 40);
  SunBalanceText.setString(std::to_string(SunBalance));
  if (runOnceUI) {
    SunBalanceText.setPosition({ 20.0, 20.0 });
    runOnceUI = false;
  }
  window->draw(SunBalanceText);

}
