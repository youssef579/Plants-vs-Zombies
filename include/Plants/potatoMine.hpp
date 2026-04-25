#pragma once

#include <Plants/Plant.hpp>

const float POTATO_MINE_TIMER_BEFORE_EXPLOSION = 7.0f;
const float POTATO_MINE_HEALTH = 1500.0f;

void updatePotatoMine(Plant& potatoMine, float dt);

void drawPotatoMine(Plant& potatoMine);
