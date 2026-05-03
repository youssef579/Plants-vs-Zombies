#include <LevelManager.hpp>
#include <AssetsManager.hpp>
#include <LevelProgress.hpp>
#include <cmath>

static float timer = 0;
void drawLevelProgress(float dt) {
  static sf::Texture &emptyProgressTexture = getTexture("assets/levelProgress/FlagMeter.png");
  static sf::Sprite emptyProgressSprite(emptyProgressTexture);

  static sf::Texture &levelProgressTexture = getTexture("assets/levelProgress/FlagMeterLevelProgress.png");
  static sf::Sprite levelProgressSprite(levelProgressTexture);

  static sf::Texture &progressBarTexture = getTexture("assets/levelProgress/ProgressBar.png");
  static sf::Sprite progressBarSprite(progressBarTexture);

  static sf::Texture &zombieHeadTexture = getTexture("assets/levelProgress/ZombieHead.png");
  static sf::Sprite zombieHeadSprite(zombieHeadTexture);

  //static float timer = 0;
  if(!(levelManager.spawningFinished && Zombie::totalZombies == 0))
    timer = levelManager.timer - levelManager.levels[levelManager.currentLevel - 1]->waves[0]->delay;
  //std::cout << timer << "\t" << levelManager.timer << "\t" << abs(levelManager.timer - timer) << "\n";

  static bool runOnce = true;
  if (runOnce) {
    emptyProgressSprite.setPosition({950, 570});
    levelProgressSprite.setPosition({990, 585});
    runOnce = false;
  }

  if (levelManager.currentWave == 0) return;

  //timer += dt;
  float offset = progressBarTexture.getSize().x * std::min(
    timer / (
      levelManager.levels[levelManager.currentLevel-1]->waves[levelManager.levels[levelManager.currentLevel-1]->numberOfWaves - 1]->delay
      - levelManager.levels[levelManager.currentLevel-1]->waves[0]->delay
    ),
    1.f
  );

  progressBarSprite.setPosition({1101 - std::floor(offset), 578.5});
  progressBarSprite.setTextureRect({{0, 0}, {(int)offset, (int)progressBarTexture.getSize().y}});

  zombieHeadSprite.setPosition({1094 - std::floor(offset), 567});

  window->draw(emptyProgressSprite);
  window->draw(progressBarSprite);
  for (int i = 0; i < levelManager.levels[levelManager.currentLevel - 1]->numberOfWaves; i++) {
    if (levelManager.levels[levelManager.currentLevel-1]->waves[i]->isBigWave)
      window->draw(levelManager.levels[levelManager.currentLevel - 1]->waves[i]->waveSprite);
  }
  window->draw(levelProgressSprite);
  window->draw(zombieHeadSprite);
}

void resetLevelProgress() {
  timer = 0.0f;
}
