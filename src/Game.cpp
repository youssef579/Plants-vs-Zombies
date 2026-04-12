#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Game.hpp>
#include <Home.hpp>
#include <UI/Overlay.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <Weather.hpp>
#include <SunManager.hpp>
#include <Plants/SunFlower.hpp>
#include <Plants/Wallnut.hpp>
#include <Array.hpp>
#include <bullet.hpp>
#include <globals.hpp>

int gameState = 0;
/*
  0 -> Home menu
*/

Array<Bullet>bullets;

sf::Clock drawClock;
float dt; // Delta Time (time between each frame draw)

static bool weatherInited = false;

bool isPaused = false;
bool runOnce = true;

void updateGame() {
  dt = drawClock.restart()
           .asSeconds(); // clock.restart() sets time to 0 and returns the last
                         // time before modifying it
  // calling dt = clock.restart() each frame returns the time between frames
  // (dt)

  switch (gameState) {
  case 0:
    updateHome();
    break;
  default:

    static sf::Texture& peaBulletTexture = getTexture("assets/bullets/pea.png");
    static sf::Texture& peaIceBulletTexture = getTexture("assets/bullets/pea_ice.png");
    static sf::Texture& peaParticlesTexture = getTexture("assets/bullets/pea_particles.png");
    static sf::Texture& peaIceParticlesTexture = getTexture("assets/bullets/peaice_particles.png");
    if (runOnce) {
      pauseMenu.init();
      gameWeather.isRaining = true;

      ////////////////////////////////////////////////////////



      //sf::Sprite test = sf::Sprite(peaIceParticlesTexture);
      //test.setPosition({ 300,300 });

      //sf::Sprite test2 = sf::Sprite(peaParticlesTexture);
      //test2.setPosition({ 0,300 });

      //sf::Sprite bull = sf::Sprite(peaBulletTexture);
      //bull.setPosition({0, 300});

      //sf::CircleShape shadow;
      //shadow.setRadius(8.f);
      //shadow.setFillColor(sf::Color(255, 0, 0, 255));
      //shadow.setScale({ 1.5f, 0.5f });
      //shadow.setOrigin({ shadow.getLocalBounds().size.x / 2.f, shadow.getLocalBounds().size.y / 2.f + 35.f });
      //test.setTextureRect(sf::IntRect({ 24 * randomRange(0,3), 0 }, { 24, 24 }));
      //test2.setTextureRect(sf::IntRect({ 24 * randomRange(0,3), 0 }, { 24, 24 }));


      /*Bullet b1 = { bull,test2,shadow,0,0,false,false,PARTICLE_DEFAULT_TIMER};
      Bullet b2 = { sf::Sprite(peaIceBulletTexture),test,shadow,0,0,false,false,PARTICLE_DEFAULT_TIMER };
      
      bullets.push(b1);
      bullets.push(b2);*/

      Bullet b1 = { sf::Sprite(peaIceBulletTexture), sf::Sprite(peaIceParticlesTexture),0,0,0,0,PARTICLE_DEFAULT_TIMER };

      music.play("DayStage");
      runOnce = false;
    }

    if (isPaused) {
      pauseMenu.update();
      pauseMenu.draw();
      break;
    }

    for (int i = 0; i < bullets.size; i++) {
      bullets[i].update(dt);
      bullets[i].draw();
    }
    
    bullets.erase([](const Bullet& b) {
      return b.remove;
    });
    
    Sun::manageSuns(dt);
    gameWeather.update(dt);
    drawUI();
    break;
  }
}


