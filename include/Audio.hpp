#pragma once

#include <SFML/Audio.hpp>
#include <Files.hpp> // To access settings



struct WeatherSystem; //forward-declaration for needed arguments

void playMusic(std::string op); //Play a different Music track (op: "Menu", "DayStage")
void updateVolume(); //Update Volume settings on all active sounds (needed when volume setting changes while sound is active)
void playSound(std::string op); //Play a Sound Effect (op: "CollectSun")
void updateSFX(); //Delete any inactive SFX from array then reorganize array to remove empty space
void initAudio(); //Initialize Sound Buffers from assets