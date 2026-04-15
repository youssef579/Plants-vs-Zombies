#pragma once

#include <Plants/Plant.hpp>

const float GENERATE_ICEBULLET_TIMER = 1.5f;

Plant createIcepea(float x, float y, int row);

void updateIcepea(Plant& peashooter, float dt);

void drawIcepea(Plant& peashooter);
