#pragma once

#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>
#include <ReAnimation/ReAnimation.hpp>

struct Sun {
  //sf::Sprite sprite;
  int value;
  enum State { Falling, FreeFalling, OnGround, Collecting, Paused, Done };
  State state;
  float groundTimer;              // Keeps track of time spent on ground
  float distanceToCollection;
  sf::Vector2f direction;         // Direction to Collection
  float fadeFactor;               // Used in fading equation
  float sunflowerlevel;
  float sunFlowerSpeed;           // the initial fallSpeed of each frame
  //Spritesheet sheet;              // Animation object
  ReAnimator reAnimator;


  //Constants
  private:
  static constexpr float fallSpeed = 60.0f;            // Changes vertical speed while falling
  static constexpr float groundY = 450.0f;             // y-position of ground
  static constexpr float groundDuration = 3.0f;        // Time stayed on ground before auto-collect
  static constexpr float collectionSpeed = 300.0f;     // Changes speed of sun while being collected
  static constexpr float collectionErrorMargin = 35.0f;
  static constexpr int assetWidth = 116;              // SunAsset width (Used for spawn bounds)
  static constexpr float spawnInterval = 8.0f;         // Amount of seconds between each sun spawn
  static constexpr float acceleration = 400.0f;
  static constexpr float distanceSunFlower = 50.0f;     // the distance the sun will fly above the sunflower

  static constexpr sf::Vector2f collectionSite = { 43, 41 };    // Position of sun collection site
  //Shared Variables
  public:
  static Array<Sun> sunArray; // Array containing all Sun objects
  static int sunBalance;                      // Amount of sun collected
  static float spawnTimer;                    // Keeps track of time between last spawn
  static bool hovering;
  static bool isSpawning;

  // Personal Functions (Related to current Sun struct)
  void collect();
  void draw();
  void update(float dt); // Returns whether a sun is alive or not (deleted)
  bool onClick();        // Returns whether a sun is hovered or not

  // Public Functions (Related to all Sun structs)
  static void manageSuns(float dt, State s = Falling); // Manages spawning , update , draw and hover
  static void drawAll();
  static void generate(sf::Vector2f pos, int val, bool isSunFlower);
  static void spawn(int val = 25);
  static void clear(); // Resets the Sun Array to be used in a new level
};
