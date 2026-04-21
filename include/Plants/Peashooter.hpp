#pragma once

#include <Plants/Plant.hpp>

const float GENERATE_BULLET_TIMER = 1.2f;
const float PEASHOOTER_HEALTH = 300.0f;

void updatePeashooter(Plant& peashooter, float dt);

void drawPeashooter(Plant& peashooter);
