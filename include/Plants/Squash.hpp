#pragma once

#include <Plants/Plant.hpp>

const float SQUASH_DAMAGE = 2000.0f;
const float SQUASH_HEALTH = 2000.0f;
const float SQUASH_DELAY_TIME = 0.2f;

void updateSquash(Plant& squash, float dt);

void drawSquash(Plant& squash);
