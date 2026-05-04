#include <Array.hpp>
#include <Audio.hpp>
#include <iostream>
#include <globals.hpp>
#include <Weather.hpp>
#include <Zombies/Zombie.hpp>

/*
 Steps to add new sfx:
   Step 1: in Audio.hpp , add a buffer and path.
   Step 2: in Audio.cpp , add to Sounds::init() your buffer.
   Step 3: in Audio.cpp , add to Sounds::play() sfx condition.
*/


Array<sf::Sound> Sounds::sfxArray;
sf::Music Music::container;
Sounds::Buffers Sounds::buffers;

void Music::play(std::string op) {
  std::string path;
  music.container.stop(); //Stop any pre-existing music first

  if      (op == "Menu")     path = music.paths.mainMenu;
  else if (op == "DayStage") path = music.paths.dayStage;

  if (!music.container.openFromFile(path)) {
    throwLoadingError(path);
    return;
  }

  music.container.setLooping(true);
  music.container.setVolume(settings.musicVolume);
  music.container.play();
}
void Music::stop() { music.container.stop(); }



void Sounds::play(std::string op) {
  sf::Sound *sound;
  //Can be made more modular later
  if (op == "CollectSun") { sound = new sf::Sound(sounds.buffers.collectSun);
        sound->setPitch(randomRange(0.9f, 1.3f)); } //Random pitch for dramatic effect :)
  else if (op == "ZombiesComing") sound = new sf::Sound(sounds.buffers.zombiesComing);
  else if (op == "Pause")         sound = new sf::Sound(sounds.buffers.pauseSound);
  else if (op == "ButtonClick")   sound = new sf::Sound(sounds.buffers.buttonClick);
  else if (op == "Tap1")          sound = new sf::Sound(sounds.buffers.tap1);
  else if (op == "Tap2")          sound = new sf::Sound(sounds.buffers.tap2);
  else if (op == "sunFlowerPop")  sound = new sf::Sound(sounds.buffers.sunFlowerPop);
  else if (op == "Shovel")        sound = new sf::Sound(sounds.buffers.shovel);
  else if (op == "Plant1")        sound = new sf::Sound(sounds.buffers.plant1);
  else if (op == "Plant2")        sound = new sf::Sound(sounds.buffers.plant2);
  else if (op == "Splat1")        sound = new sf::Sound(sounds.buffers.splat1);
  else if (op == "Splat2")        sound = new sf::Sound(sounds.buffers.splat2);
  else if (op == "Splat3")        sound = new sf::Sound(sounds.buffers.splat3);
  else if (op == "Throw1")        sound = new sf::Sound(sounds.buffers.throw1);
  else if (op == "Throw2")        sound = new sf::Sound(sounds.buffers.throw2);
  else if (op == "ThrowSnowPea")  sound = new sf::Sound(sounds.buffers.throwSnowPea);
  else if (op == "ZombieBite")    sound = new sf::Sound(sounds.buffers.zombieBite);
  else if (op == "ZombieGulp")    sound = new sf::Sound(sounds.buffers.zombieGulp);
  else if (op == "CherryBomb")    sound = new sf::Sound(sounds.buffers.cherryBomb);
  else if (op == "Jalapeno")      sound = new sf::Sound(sounds.buffers.jalapeno);
  else if (op == "Lawnmower")     sound = new sf::Sound(sounds.buffers.lawnmower);
  else if (op == "PotatoMine")    sound = new sf::Sound(sounds.buffers.potatoMine);
  else if (op == "WinMusic")      sound = new sf::Sound(sounds.buffers.winMusic);
  else if (op == "LoseMusic")     sound = new sf::Sound(sounds.buffers.loseMusic);
  else if (op == "IceShroom")     sound = new sf::Sound(sounds.buffers.iceShroom);
  else if (op == "SquashHmm")     sound = new sf::Sound(sounds.buffers.squashHmm);
  else if (op == "SquashHmm2")    sound = new sf::Sound(sounds.buffers.squashHmm2);
  else if (op == "Awooga")        sound = new sf::Sound(sounds.buffers.awooga);
  else if (op == "Groan1")        sound = new sf::Sound(sounds.buffers.groan1);
  else if (op == "Groan2")        sound = new sf::Sound(sounds.buffers.groan2);
  else if (op == "Groan3")        sound = new sf::Sound(sounds.buffers.groan3);
  else if (op == "Groan4")        sound = new sf::Sound(sounds.buffers.groan4);
  else if (op == "Groan5")        sound = new sf::Sound(sounds.buffers.groan5);
  else if (op == "Groan6")        sound = new sf::Sound(sounds.buffers.groan6);
  else if (op == "Click")         sound = new sf::Sound(sounds.buffers.click);
  else {
    std::cerr << "FATAL ERROR: Unkown sfx option \"" << op << "\" detected" << std::endl;
    std::system("pause");
    exit(1);
    return;
  }

  sound->setVolume(settings.soundFXVolume);
  sound->play();

  sounds.sfxArray.push(*sound);
  update();
}

void Sounds::init() {
  sounds.buffers.collectSun    = getSoundBuffer(sounds.paths.collectSun);
  sounds.buffers.zombiesComing = getSoundBuffer(sounds.paths.zombiesComing);
  sounds.buffers.pauseSound    = getSoundBuffer(sounds.paths.pauseSound);
  sounds.buffers.buttonClick   = getSoundBuffer(sounds.paths.buttonClick);
  sounds.buffers.tap1          = getSoundBuffer(sounds.paths.tap1);
  sounds.buffers.tap2          = getSoundBuffer(sounds.paths.tap2);
  sounds.buffers.sunFlowerPop  = getSoundBuffer(sounds.paths.sunFlowerPop);
  sounds.buffers.shovel        = getSoundBuffer(sounds.paths.shovel);
  sounds.buffers.plant1        = getSoundBuffer(sounds.paths.plant1);
  sounds.buffers.plant2        = getSoundBuffer(sounds.paths.plant2);
  sounds.buffers.splat1        = getSoundBuffer(sounds.paths.splat1);
  sounds.buffers.splat2        = getSoundBuffer(sounds.paths.splat2);
  sounds.buffers.splat3        = getSoundBuffer(sounds.paths.splat3);
  sounds.buffers.throw1        = getSoundBuffer(sounds.paths.throw1);
  sounds.buffers.throw2        = getSoundBuffer(sounds.paths.throw2);
  sounds.buffers.throwSnowPea  = getSoundBuffer(sounds.paths.throwSnowPea);
  sounds.buffers.zombieBite    = getSoundBuffer(sounds.paths.zombieBite);
  sounds.buffers.zombieGulp    = getSoundBuffer(sounds.paths.zombieGulp);
  sounds.buffers.cherryBomb    = getSoundBuffer(sounds.paths.cherryBomb);
  sounds.buffers.jalapeno      = getSoundBuffer(sounds.paths.jalapeno);
  sounds.buffers.lawnmower     = getSoundBuffer(sounds.paths.lawnmower);
  sounds.buffers.potatoMine    = getSoundBuffer(sounds.paths.potatoMine);
  sounds.buffers.winMusic      = getSoundBuffer(sounds.paths.winMusic);
  sounds.buffers.loseMusic     = getSoundBuffer(sounds.paths.loseMusic);
  sounds.buffers.iceShroom     = getSoundBuffer(sounds.paths.iceShroom);
  sounds.buffers.squashHmm     = getSoundBuffer(sounds.paths.squashHmm);
  sounds.buffers.squashHmm2    = getSoundBuffer(sounds.paths.squashHmm2);
  sounds.buffers.awooga        = getSoundBuffer(sounds.paths.awooga);
  sounds.buffers.groan1        = getSoundBuffer(sounds.paths.groan1);
  sounds.buffers.groan2        = getSoundBuffer(sounds.paths.groan2);
  sounds.buffers.groan3        = getSoundBuffer(sounds.paths.groan3);
  sounds.buffers.groan4        = getSoundBuffer(sounds.paths.groan4);
  sounds.buffers.groan5        = getSoundBuffer(sounds.paths.groan5);
  sounds.buffers.groan6        = getSoundBuffer(sounds.paths.groan6);
  sounds.buffers.click         = getSoundBuffer(sounds.paths.click);

}

void Sounds::update() {
  sounds.sfxArray.erase([](sf::Sound &sound) {
    return sound.getStatus() == sf::Sound::Status::Stopped;
  });
}

void Sounds::updateVolume() {
  music.container.setVolume(settings.musicVolume);
  for (int i = 0; i < sounds.sfxArray.size; i++)
    sounds.sfxArray[i].setVolume(settings.soundFXVolume);
  gameWeather.updateVolume();
  Zombie::updateVolumes();
}

Sounds sounds;
Music music;



void initAudio() {
  sounds.init();
}
