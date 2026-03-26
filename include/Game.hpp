#pragma once

#include <globals.hpp>
extern int gameState;
void updateGame();


//Sun Management
extern int SunBalance;
void generateSun(float x, float y, int value); //generates sun object at position {x, y}
void spawnSun(int value=50); //calls generateSun() at a random location
void collectSun(Sun* sun); //Changes state to 2(collecting) and calculates initial values needed
void manageSuns(); //calls all functions related to managing active suns
void updateSun(); //Handle sun movement/collection according to its state
void drawSun();



void updatePause(); //Handles Pause Menu UI and Logic
void drawUI();