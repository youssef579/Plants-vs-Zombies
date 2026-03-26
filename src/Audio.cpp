#include <Audio.hpp>
#include <iostream>
#include <globals.hpp>

const std::string MainMenuMusicPath = "assets/Music_MainMenu.ogg";
const std::string DayStagePath = "assets/Music_DayStage.ogg";



const float DEFAULT_MUSIC_VOLUME = 75.0f;
const float DEFAULT_SOUNDFX_VOLUME = 75.0f;

float Settings_MusicVolume = DEFAULT_MUSIC_VOLUME;
float Settings_SoundFXVolume = DEFAULT_SOUNDFX_VOLUME;

sf::Music music;

sf::SoundBuffer soundBuffer_collectSun;
sf::SoundBuffer soundBuffer_zombiesComing;

const int SFX_ARRAY_SIZE = 255; //max simultaneous sound effects
sf::Sound* sfxArray[SFX_ARRAY_SIZE] = { nullptr };
int sfxArrayCounter = 0;


void playMusic(std::string op) {
  std::string path;
  music.stop(); //Stop any pre-existing music first

  if (op == "Menu") path = MainMenuMusicPath;
  else if (op == "DayStage") path = DayStagePath;

  if (!music.openFromFile(path)) {
    //throwLoadingError(path);
    std::cerr << "FATAL ERROR: Could not load " << path << std::endl;
    std::cerr << "Check if the file exists in the correct folder!" << std::endl;
    std::system("pause");
    exit(1);
    return;
  }

  music.setLooping(true);
  music.setVolume(Settings_MusicVolume);
  music.play();
}

void updateVolume(float newMusic, float newSoundFX) {
  music.setVolume(newMusic);
  for (int i = 0; i < sfxArrayCounter; i++) {
    sfxArray[i]->setVolume(newSoundFX);
  }
}

void initAudio() { //Initialize Sound Buffers for SFX
  bool hasNoError = false;
  hasNoError = soundBuffer_collectSun.loadFromFile("assets/sfx_collectSun.mp3") &&
    soundBuffer_zombiesComing.loadFromFile("assets/sfx_zombiesComing.mp3");
  //hasAnyError = sb1.load() && sb2.load() && .... 
  if (!hasNoError) {
    //throwLoadingError(MainMenuMusicPath);
    std::cerr << "FATAL ERROR: Could not load a Sound File" << std::endl;
    std::cerr << "Check if the file exists in the correct folder!" << std::endl;
    std::system("pause");
    exit(1);
    return;
  }


}


void playSound(std::string op) {
  sf::Sound* sound;
  //Can be made more modular later
  if (op == "CollectSun") { sound = new sf::Sound(soundBuffer_collectSun); sound->setPitch(randomRange(0.9f, 1.3f)); } //Random pitch for dramatic effect :)
  else if (op == "ZombiesComing") sound = new sf::Sound(soundBuffer_zombiesComing);
  else {
    std::cerr << "FATAL ERROR: Unkown sfx option \"" << op << "\" detected" << std::endl;
    std::system("pause");
    exit(1);
    return;
  }
  sound->setVolume(Settings_SoundFXVolume);
  sound->play();

  if (sfxArrayCounter >= SFX_ARRAY_SIZE) { //Prevent possible out of bounds error
    std::cerr << "FATAL ERROR: Maximum simultaneous SFX reached (" << SFX_ARRAY_SIZE << ")" << std::endl;
    std::system("pause");
    exit(1);
    return;
  }

  sfxArray[sfxArrayCounter] = sound;
  sfxArrayCounter++;
  updateSFX();

}


void updateSFX() {
  for (int i = 0; i < sfxArrayCounter; ++i)
  {
    if (sfxArray[i]->getStatus() == sf::Sound::Status::Stopped) //if sfx has finished
    {
      delete sfxArray[i];
      sfxArray[i] = nullptr;

      for (int j = i; j < sfxArrayCounter - 1; j++) //reorganize the array
      {
        sfxArray[j] = sfxArray[j + 1];
      }

      sfxArray[sfxArrayCounter - 1] = nullptr;
      sfxArrayCounter--;
      i--; //check sfxArray[i] again (new value)
    }
  }
}