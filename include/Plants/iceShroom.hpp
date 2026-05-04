#pragma once

#include <Plants/Plant.hpp>

const float ICESHROOM_EXPLOSION_TIMER = 0.466f;
const float ICESHROOM_HEALTH = 2000.0f;

void updateIceShroom(Plant &iceShroom, float dt);

void drawIceShroom(Plant &iceShroom);
