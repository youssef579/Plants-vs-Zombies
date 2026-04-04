#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Weather.hpp>
#include <globals.hpp>
#include <iostream>

const std::string MainMenuMusicPath = "assets/sounds/Music_MainMenu.ogg",
                  DayStagePath = "assets/sounds/Music_DayStage.ogg";

const float DEFAULT_MUSIC_VOLUME = 75.0f, DEFAULT_SOUNDFX_VOLUME = 75.0f,
            DEFAULT_WEATHERFX_VOLUME = 75.0f;

float settingsMusicVolume = DEFAULT_MUSIC_VOLUME,
      settingsSoundFXVolume = DEFAULT_SOUNDFX_VOLUME,
      settingsWeatherFXVolume = DEFAULT_WEATHERFX_VOLUME;

sf::Music music;

sf::SoundBuffer *soundBuffer_collectSun, *soundBuffer_zombiesComing,
    *soundBuffer_pauseSound, *soundBuffer_buttonClick, *soundBuffer_tap1,
    *soundBuffer_tap2;

const int SFX_ARRAY_SIZE = 255; // max simultaneous sound effects
sf::Sound *sfxArray[SFX_ARRAY_SIZE] = {nullptr};
int sfxArrayCounter = 0;

void playMusic(std::string op) {
  std::string path;
  music.stop(); // Stop any pre-existing music first

  if (op == "Menu")
    path = MainMenuMusicPath;
  else if (op == "DayStage")
    path = DayStagePath;

  if (!music.openFromFile(path))
    throwLoadingError(path);

  music.setLooping(true);
  music.setVolume(settingsMusicVolume);
  music.play();
}

void updateVolume(WeatherSystem *ws) {
  music.setVolume(settingsMusicVolume);
  for (int i = 0; i < sfxArrayCounter; i++) {
    sfxArray[i]->setVolume(settingsSoundFXVolume);
  }
  if (ws)
    ws->updateVolume();
}

void initAudio() { // Initialize Sound Buffers for SFX
  soundBuffer_collectSun = &getSound("assets/sounds/sfx_collectSun.mp3");
  soundBuffer_zombiesComing = &getSound("assets/sounds/sfx_zombiesComing.mp3");
  soundBuffer_pauseSound = &getSound("assets/sounds/sfx_pause.ogg");
  soundBuffer_buttonClick = &getSound("assets/sounds/sfx_buttonClick.ogg");
  soundBuffer_tap1 = &getSound("assets/sounds/sfx_tap1.ogg");
  soundBuffer_tap2 = &getSound("assets/sounds/sfx_tap2.ogg");
}

void playSound(std::string op) {
  sf::Sound *sound;

  // Can be made more modular later
  if (op == "CollectSun") {
    sound = new sf::Sound(*soundBuffer_collectSun);
    sound->setPitch(randomRange(0.9f, 1.3f));
  } // Random pitch for dramatic effect :)
  else if (op == "ZombiesComing")
    sound = new sf::Sound(*soundBuffer_zombiesComing);
  else if (op == "Pause")
    sound = new sf::Sound(*soundBuffer_pauseSound);
  else if (op == "ButtonClick")
    sound = new sf::Sound(*soundBuffer_buttonClick);
  else if (op == "Tap1")
    sound = new sf::Sound(*soundBuffer_tap1);
  else if (op == "Tap2")
    sound = new sf::Sound(*soundBuffer_tap2);
  else {
    std::cerr << "FATAL ERROR: Unkown sfx option \"" << op << "\" detected"
              << std::endl;
    std::system("pause");
    exit(1);
    return;
  }
  sound->setVolume(settingsSoundFXVolume);
  sound->play();

  if (sfxArrayCounter >=
      SFX_ARRAY_SIZE) { // Prevent possible out of bounds error
    std::cerr << "FATAL ERROR: Maximum simultaneous SFX reached ("
              << SFX_ARRAY_SIZE << ")" << std::endl;
    std::system("pause");
    exit(1);
    return;
  }

  sfxArray[sfxArrayCounter] = sound;
  sfxArrayCounter++;
  updateSFX();
}

void updateSFX() {
  for (int i = 0; i < sfxArrayCounter; ++i) {
    if (sfxArray[i]->getStatus() ==
        sf::Sound::Status::Stopped) { // if sfx has finished
      delete sfxArray[i];
      sfxArray[i] = nullptr;

      for (int j = i; j < sfxArrayCounter - 1; j++) // reorganize the array
        sfxArray[j] = sfxArray[j + 1];

      sfxArray[sfxArrayCounter - 1] = nullptr;
      sfxArrayCounter--;
      i--; // check sfxArray[i] again (new value)
    }
  }
}
