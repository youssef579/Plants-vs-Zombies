#pragma once
//#include <SunManager.hpp>
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>


extern int SunBalance;


void manageSuns(float dt);
void drawSun();
void updateSun(float dt);
void generateSun(float x, float y, int value);
void spawnSun(int value=25);
void collectSun(Sun* sun);