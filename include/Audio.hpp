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
      sunFlowerPop,
      shovel,
      plant1,
      plant2,
      splat1,
      splat2,
      splat3,
      throw1,
      throw2,
      throwSnowPea,
      zombieBite,
      zombieGulp;
  };

  struct Paths {
    inline static const std::string
      collectSun      = "assets/sounds/sfx_collectSun.mp3",
      zombiesComing   = "assets/sounds/sfx_zombiesComing.mp3",
      pauseSound      = "assets/sounds/sfx_pause.ogg",
      buttonClick     = "assets/sounds/sfx_buttonClick.ogg",
      tap1            = "assets/sounds/sfx_tap1.ogg",
      tap2            = "assets/sounds/sfx_tap2.ogg",
      sunFlowerPop    = "assets/sounds/sfx_sunFlowerPop.wav",
      shovel          = "assets/sounds/sfx_shovel.ogg",
      plant1          = "assets/sounds/sfx_plant1.ogg",
      plant2          = "assets/sounds/sfx_plant2.ogg",
      splat1          = "assets/sounds/sfx_splat1.ogg",
      splat2          = "assets/sounds/sfx_splat2.ogg",
      splat3          = "assets/sounds/sfx_splat3.ogg",
      throw1          = "assets/sounds/sfx_throw1.ogg",
      throw2          = "assets/sounds/sfx_throw2.ogg",
      throwSnowPea    = "assets/sounds/sfx_throwSnowpea.ogg",
      zombieBite      = "assets/sounds/sfx_zombieBite.ogg",
      zombieGulp      = "assets/sounds/sfx_zombieGulp.ogg";
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
