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


sf::Sound* Sounds::sfxArray[Sounds::sfxArraySize];
int Sounds::sfxArrayCntr = 0;
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
  else {
    std::cerr << "FATAL ERROR: Unkown sfx option \"" << op << "\" detected" << std::endl;
    std::system("pause");
    exit(1);
    return;
  }

  sound->setVolume(settings.soundFXVolume);
  sound->play();

  // Save sound obj in array
  if (sounds.sfxArrayCntr >= sounds.sfxArraySize) { //Prevent possible out of bounds error
    std::cerr << "FATAL ERROR: Maximum simultaneous SFX reached (" << sounds.sfxArraySize << ")" << std::endl;
    std::system("pause");
    exit(1);
    return;
  }

  sounds.sfxArray[sounds.sfxArrayCntr] = sound;
  sounds.sfxArrayCntr++;
  update();
}

void Sounds::init() {
  sounds.buffers.collectSun    = getSound(sounds.paths.collectSun);
  sounds.buffers.zombiesComing = getSound(sounds.paths.zombiesComing);
  sounds.buffers.pauseSound    = getSound(sounds.paths.pauseSound);
  sounds.buffers.buttonClick   = getSound(sounds.paths.buttonClick);
  sounds.buffers.tap1          = getSound(sounds.paths.tap1);
  sounds.buffers.tap2          = getSound(sounds.paths.tap2);
  sounds.buffers.sunFlowerPop  = getSound(sounds.paths.sunFlowerPop);

}

void Sounds::update() {
  for (int i = 0; i < sounds.sfxArrayCntr; ++i)
  {
    if (sounds.sfxArray[i]->getStatus() == sf::Sound::Status::Stopped) //if sfx has finished
    {
      delete sounds.sfxArray[i];
      sounds.sfxArray[i] = nullptr;

      for (int j = i; j < sounds.sfxArrayCntr - 1; j++) //reorganize the array
      {
        sounds.sfxArray[j] = sounds.sfxArray[j + 1];
      }

      sounds.sfxArray[sounds.sfxArrayCntr - 1] = nullptr;
      sounds.sfxArrayCntr--;
      i--; //check sfxArray[i] again (new value)
    }
  }
}

void Sounds::updateVolume() {
  music.container.setVolume(settings.musicVolume);
  for (int i = 0; i < sounds.sfxArrayCntr; i++) {
    sounds.sfxArray[i]->setVolume(settings.soundFXVolume);
  }
  gameWeather.updateVolume();

}

Sounds sounds;
Music music;



void initAudio() {
  sounds.init();
}
