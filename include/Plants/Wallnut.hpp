#pragma once
#include <Plants/Plant.hpp>
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>

Plant createWallnut(float x, float y);

void updateWallnut(Plant &sunFlower, float dt);

void drawWallnut(Plant &sunFlower);