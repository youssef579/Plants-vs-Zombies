#pragma once
#include <Plants/Plant.hpp>
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>

const float GENERATE_SUN_FLOWER_INTERVAL = 10.0f;

Plant createSunFlower(float x, float y);

void updateSunFlower(Plant &sunFlower, float dt);

void drawSunFlower(Plant &sunFlower, float dt);