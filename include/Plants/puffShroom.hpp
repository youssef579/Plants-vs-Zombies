#pragma once

#include <Plants/Plant.hpp>

const float GENERATE_PUFFSHROOM_BULLET_TIMER = 1.5f;
const float PUFFSHROOM_HEALTH = 300.0f;

void updatePuffShroom(Plant& puffShroom, float dt);

void drawPuffShroom(Plant& puffShroom);
