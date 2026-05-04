#pragma once

#include <Plants/Plant.hpp>

const float GENERATE_REPEATERBULLET_TIMER = 1.2f;
const float GENERATE_SECOND_REPEATERBULLET_TIMER = 0.15f;
const float REPEATERPEA_HEALTH = 300.0f;

void updateRepeaterpea(Plant &repeaterpea, float dt);

void drawRepeaterpea(Plant &repeaterpea);
