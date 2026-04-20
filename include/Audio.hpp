#pragma once

#include <Array.hpp>
#include <SFML/Audio.hpp>
#include <Files.hpp> // To access settings
#include <AssetsManager.hpp>


struct Music {
  static sf::Music container;
  struct Paths {
    inline static const std::string
      mainMenu = "assets/sounds/Music_MainMenu.ogg",
      dayStage = "assets/sounds/Music_DayStage.ogg";
  };
  static Paths paths;

  static void play(std::string);
};

struct Sounds {
  static Array<sf::Sound> sfxArray;

  struct Buffers {
    sf::SoundBuffer
      collectSun,
      zombiesComing,
      pauseSound,
      buttonClick,
      tap1,
      tap2,
      sunFlowerPop;
  };

  struct Paths {
    inline static const std::string
      collectSun = "assets/sounds/sfx_collectSun.mp3",
      zombiesComing = "assets/sounds/sfx_zombiesComing.mp3",
      pauseSound = "assets/sounds/sfx_pause.ogg",
      buttonClick = "assets/sounds/sfx_buttonClick.ogg",
      tap1 = "assets/sounds/sfx_tap1.ogg",
      tap2 = "assets/sounds/sfx_tap2.ogg",
      sunFlowerPop = "assets/sounds/sfx_sunFlowerPop.wav";
  };


  static Buffers buffers;
  static Paths paths;

  static void init();
  static void play(std::string);
  static void update();
  static void updateVolume();

};

extern Sounds sounds;
extern Music music;

struct WeatherSystem; //forward-declaration for needed arguments


void initAudio();
//void playMusic(std::string op); //Play a different Music track (op: "Menu", "DayStage")
//void updateVolume(); //Update Volume settings on all active sounds (needed when volume setting changes while sound is active)
//void playSound(std::string op); //Play a Sound Effect (op: "CollectSun")
//void updateSFX(); //Delete any inactive SFX from array then reorganize array to remove empty space
//void initAudio(); //Initialize Sound Buffers from assets
