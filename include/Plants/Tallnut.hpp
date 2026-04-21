#pragma once
#include <Plants/Plant.hpp>
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>

const float TALLNUT_HEALTH = 8000.0f;
const float TALLNUT_CRACK1_HEALTH = 5333.3f;
const float TALLNUT_CRACK2_HEALTH = 2666.6f;

void updateTallnut(Plant &tallnut, float dt);

void drawTallnut(Plant &tallnut);
