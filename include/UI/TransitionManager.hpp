#pragma once
#include <functional>
#include <Window.hpp>

struct TransitionManager {

  static std::function<void()> action;
  static float timer;
  static bool state;  // fade out / fade in
  static bool active;
  static float maxRadius;
  static float duration;
  static sf::CircleShape circle;

  static void start(std::function<void()> _action);

  static void update(float dt);

  static void draw();



};
