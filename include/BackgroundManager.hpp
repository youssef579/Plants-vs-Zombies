#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>



struct DirtParticle {
  sf::Sprite* sprite;
  sf::Vector2f velocity;
  float lifetime;
  float rotationSpeed;
  bool active = false;
};


struct LevelManager {

  enum levelType {Day, Night};

  sf::Texture backGroundTexture;
  sf::Texture grassTexture;
  sf::Texture threeMiddleGrassTexture;
  sf::Texture fullGrassTexture;
  sf::Texture rollTexture;
  sf::Texture capTexture;
  sf::Texture dirtTexture;

  sf::Sprite* backGroundSprite = nullptr;
  sf::Sprite* threeMiddleGrassSprite = nullptr;
  sf::Sprite* fullGrassSprite = nullptr;
  sf::Sprite* grassSprites[5] = { nullptr };
  sf::Sprite* rollSprites[5] = { nullptr };
  sf::Sprite* capSprites[5] = { nullptr };

  sf::SoundBuffer* dirtBuffer = nullptr;
  sf::Sound* dirtSound = nullptr;

  sf::View camera;
  float introTimer = 0.0f;
  bool isIntroRunning = true;

  float currentX[5];
  bool isRolling[5];
  bool isThreeMiddleRunning = false;
  bool shouldStartRolling[5];
  float timers[5];
  bool timerFinished[5];
  bool isThreeMiddleFinished = false;
  bool isFullGrassRunning = false;
  bool isFullGrassFinished = false;
  bool dirtSoundStarted = false;
  float fullGrassCurrentX = 0.0f;
  std::vector <DirtParticle> dirtPool;

  float targetX = 0.0f;
  float threeMiddleCurrentX = 0.0f;
  static constexpr float rollSpeed = 200.0f;
  float startX = 200.0f;
  float groundY[5] = { 160.0f , 233.0f , 310.0f , 385.0f , 460.0f };

  levelType type;

  void init(levelType);
  void startPlanting();
  void update(float dt);
  void draw(sf::RenderWindow& window);
  void spawnDirt(sf::Vector2f position);
  void updateDirt(float dt);
 
};


extern LevelManager dayLevel;
