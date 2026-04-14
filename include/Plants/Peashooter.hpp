#pragma once
#include <Plants/Plant.hpp>

const float GENERATE_BULLET = 1.5f;
Plant createPeashooter(float x, float y, float row);
void updatePeashooter(Plant& peashooter, float dt);
void drawPeashooter(Plant& peashooter);
