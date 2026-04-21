#include <Array.hpp>
#include <Audio.hpp>
#include <iostream>
#include <globals.hpp>
#include <Weather.hpp>

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

  if (op == "Menu") path = music.paths.mainMenu;
  else if (op == "DayStage") path = music.paths.dayStage;

  if (!music.container.openFromFile(path)) {
    throwLoadingError(path);
    return;
  }

  music.container.setLooping(true);
  music.container.setVolume(settings.musicVolume);
  music.container.play();
}



void Sounds::play(std::string op) {
  sf::Sound *sound;
  //Can be made more modular later
  if (op == "CollectSun") { sound = new sf::Sound(sounds.buffers.collectSun);
        sound->setPitch(randomRange(0.9f, 1.3f)); } //Random pitch for dramatic effect :)
  else if (op == "ZombiesComing") sound = new sf::Sound(sounds.buffers.zombiesComing);
  else if (op == "Pause") sound = new sf::Sound(sounds.buffers.pauseSound);
  else if (op == "ButtonClick") sound = new sf::Sound(sounds.buffers.buttonClick);
  else if (op == "Tap1") sound = new sf::Sound(sounds.buffers.tap1);
  else if (op == "Tap2") sound = new sf::Sound(sounds.buffers.tap2);
  else if (op == "sunFlowerPop") sound = new sf::Sound(sounds.buffers.sunFlowerPop);
  else if (op == "Shovel") sound = new sf::Sound(sounds.buffers.shovel);
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
  sounds.buffers.shovel  = getSoundBuffer(sounds.paths.shovel);

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

}

Sounds sounds;
Music music;



void initAudio() {
  sounds.init();
}
