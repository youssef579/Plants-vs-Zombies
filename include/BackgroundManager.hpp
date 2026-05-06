#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <PlantSelector.hpp>
#include <LevelManager.hpp>

struct DirtParticle {
  sf::Sprite* sprite; 
  sf::Vector2f velocity;      // Speed and direction of particles (s,y axes)
  float lifetime;             // How long particle will remain on screen before dissappearing
  float rotationSpeed;        // How fast the particles rotates while in air 
  bool active = false;        // To know if the particle is in use now 
};


struct BackgroundManager {

  enum State {Intro, Game, GameOver}; // Intro and Game not implemented yet LATER
  State state = Game;
  float gameOverTimer = 0;
  float zombiesWonTimer = 0; 
  int deathRow = 2;
  float iceFlashTimer = 0.0f;
  const float iceFlashDuration = 0.75f;

  sf::Texture backGroundTexture;
  sf::Texture grassTexture;
  sf::Texture threeMiddleGrassTexture;
  sf::Texture fullGrassTexture;
  sf::Texture rollTexture;
  sf::Texture capTexture;
  sf::Texture dirtTexture;
  sf::Texture zombiesWonTexture;

  sf::Sprite* backGroundSprite = nullptr;
  sf::Sprite* threeMiddleGrassSprite = nullptr;
  sf::Sprite* fullGrassSprite = nullptr;
  sf::Sprite* grassSprites[5] = { nullptr };    // array of sprites for individual grass row 
  sf::Sprite* rollSprites[5] = { nullptr };     
  sf::Sprite* capSprites[5] = { nullptr };
  sf::Sprite *zombiesWon = nullptr;             // Game over msg

  sf::SoundBuffer* dirtBuffer = nullptr;       
  sf::Sound* dirtSound = nullptr;

  sf::View camera;
  float introTimer = 0.0f;                       
  bool isIntroRunning = true;

  float currentX[5];
  bool isRolling[5];
  bool isThreeMiddleRunning = false;
  bool shouldStartRolling[5];              // flag to trigger the start of roll animation 
  float timers[5];                         // timers for roll delays
  bool timerFinished[5];                   // check if delay of roll delays finished
  bool isThreeMiddleFinished = false; 
  bool isFullGrassRunning = false;
  bool isFullGrassFinished = false;
  bool dirtSoundStarted = false;           // Flag to prevent the dirt sound from overlapping itself
  float fullGrassCurrentX = 0.0f;       
  bool introSlidingStarted = false;
  bool isWaitingForPlay = false;
  bool playBtnClicked = false;
  std::vector <DirtParticle> dirtPool;

  float targetX = 0.0f;
  float threeMiddleCurrentX = 0.0f;
  static constexpr float rollSpeed = 200.0f;
  float startX = 200.0f;                  // start grass X-axis position 
  float groundY[5] = { 160.0f , 233.0f , 310.0f , 385.0f , 460.0f }; // Y-axis for 5 lanes 

  LevelManager::Level::Location type; 

  void init(LevelManager::Level::Location Type);
  void startPlanting();
  void update(float dt);
  void draw(sf::RenderWindow& window);
  void drawOverlays(sf::RenderWindow &window);
  void spawnDirt(sf::Vector2f position);
  void updateDirt(float dt);
  void playGameOverScreen(int deathRow);
  void updateGameOverScreen(float dt);
  void rollOneGrass(float dt);
  void rollThreeGrass(float dt);
  void rollFiveGrass(float dt);
};


extern BackgroundManager dayLevel;


void testKeybinds(std::string key);
