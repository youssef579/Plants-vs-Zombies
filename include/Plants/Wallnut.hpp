#pragma once
#include <Plants/Plant.hpp>
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>

const float WALLNUT_HEALTH = 4000.0f;
const float WALLNUT_CRACK1_HEALTH = 2666.6f;
const float WALLNUT_CRACK2_HEALTH = 1333.3f;

void updateWallnut(Plant &sunFlower, float dt);

void drawWallnut(Plant &sunFlower);
