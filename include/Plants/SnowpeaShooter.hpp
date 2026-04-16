#pragma once

#include <Plants/Plant.hpp>

const float GENERATE_SNOWBULLET_TIMER = 1.5f;
const float SNOWPEASHOOTER_HEALTH = 50.0f;


Plant createSnowpeaShooter(float x, float y, int row);

void updateSnowpeaShooter(Plant &snowpeashooter, float dt);

void drawSnowpeaShooter(Plant &snowpeashooter);
