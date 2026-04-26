#pragma once

struct Settings {
  static float musicVolume;
  static float soundFXVolume;
  static float weatherFXVolume;
  static int timeModifier; // not saved in storage

  static bool weatherActive;
  static bool fullscreen;


  struct Default { // Default settings values
    static constexpr float musicVolume     = 75.0f;
    static constexpr float soundFXVolume   = 75.0f;
    static constexpr float weatherFXVolume = 75.0f;

    static constexpr int timeModifier      = 1;

    static constexpr bool  weatherActive   = true;
    static constexpr bool  fullscreen      = false;
  };
  static Default DEFAULT;

};

extern Settings settings;


void initFiles();
void loadPlayerData();
void loadLevelsFiles();
void loadSettingsFile();
void updateFiles();

const int MAX_LEVELS = 18;
const int LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE = 6;

extern int levelSelectorCurrentPage;
extern int maxLevelUnlocked;
