#pragma once
#include <Plants/Plant.hpp>
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>

const float GENERATE_SUN_FLOWER_INTERVAL = 10.0f;
const float SHINE_TIME = 2.0f;
const float SUN_FLOWER_HEALTH = 40.0f;

Plant createSunFlower(float x, float y, int row);

void updateSunFlower(Plant &sunFlower, float dt);

void drawSunFlower(Plant &sunFlower);
