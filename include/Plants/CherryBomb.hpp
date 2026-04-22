#pragma once

#include <Plants/Plant.hpp>

const float CHERRY_EXPLOSION_TIMER = 0.5f;
const float CHERRY_HEALTH = 2000.0f;

void updateCherryBomb(Plant& cherryBomb, float dt);

void drawCherryBomb(Plant& cherryBomb);
