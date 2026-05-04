#pragma once

#include <Plants/Plant.hpp>

const float GENERATE_SNOWBULLET_TIMER = 1.2f;
const float SNOWPEASHOOTER_HEALTH = 300.0f;

void updateSnowpeaShooter(Plant &snowpeashooter, float dt);

void drawSnowpeaShooter(Plant &snowpeashooter);
