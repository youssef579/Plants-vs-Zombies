#pragma once

#include <Plants/Plant.hpp>

const float JALAPENO_EXPLOSION_TIMER = 0.466f;
const float JALAPENO_HEALTH = 2000.0f;


void updateJalapeno(Plant& jalapeno, float dt);

void drawJalapeno(Plant& jalapeno);
