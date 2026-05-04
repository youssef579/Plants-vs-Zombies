#pragma once
#include <Array.hpp>
#include <Grid.hpp>
#include <ReAnimation/ReAnimation.hpp>
#include <Zombies/Zombie.hpp>

struct LawnMower {

  ReAnimator reAnimator;

  bool isActivated = false;
  bool remove = false;

  static void init();
  static void updateAll(float dt);
  static void drawAll();
  static void activateLawnMower(int row);

  static Array<LawnMower> lawnMowers;
};
