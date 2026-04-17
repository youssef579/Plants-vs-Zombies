#pragma once
#include <Plants/Plant.hpp>
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>

const float WALLNUT_HEALTH = 100.0f;
const float CRACK1_HEALTH = 50.0f;
const float CRACK2_HEALTH = 20.0f;

//Plant createWallnut(float x, float y, int row);

void updateWallnut(Plant &sunFlower, float dt);

void drawWallnut(Plant &sunFlower);
