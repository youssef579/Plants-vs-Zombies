#pragma once

#include <Plants/Plant.hpp>

const float GENERATE_BULLET_TIMER = 1.5f;

Plant createPeashooter(float x, float y, int row);

void updatePeashooter(Plant& peashooter, float dt);

void drawPeashooter(Plant& peashooter);
