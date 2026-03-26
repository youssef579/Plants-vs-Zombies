#pragma once

#include <SFML/Audio.hpp>

extern float Settings_MusicVolume;
extern float Settings_SoundFXVolume;
extern const float DEFAULT_MUSIC_VOLUME;
extern const float DEFAULT_SOUNDFX_VOLUME;


void playMusic(std::string op); //Play a different Music track (op: "Menu", "DayStage")
void updateVolume(float newMusic, float newSoundFX); //Update Volume settings on all active sounds (needed when volume setting changes while sound is active)
void playSound(std::string op); //Play a Sound Effect (op: "CollectSun")
void updateSFX(); //Delete any inactive SFX from array then reorganize array to remove empty space
void initAudio(); //Initialize Sound Buffers from assets