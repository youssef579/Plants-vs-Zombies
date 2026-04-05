#pragma once
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>

struct Sun {
  sf::Sprite sprite;
  int value;
  static enum State { Falling, OnGround, Collecting, Paused };
  State state;
  float groundTimer;              // Keeps track of time spent on ground
  float distanceToCollection;
  sf::Vector2f direction;         // Direction to Collection
  float fadeFactor;               // Used in fading equation
  int index;                      // Index of Sun object in sunArray[]
  Spritesheet sheet;              // Animation object

  //Constants
  private:
  static constexpr int 
    MAX_SIMULTANEOUS_SUN = 255;   // Max simultaneous sun objects
  static constexpr float
    fallSpeed = 60.0f;            // Changes vertical speed while falling
  static constexpr float          
    groundY = 450.0f;             // y-position of ground
  static constexpr float          
    groundDuration = 3.0f;        // Time stayed on ground before auto-collect
  static constexpr float          
    collectionSpeed = 300.0f;     // Changes speed of sun while being collected
  static constexpr float
    collectionErrorMargin = 35.0f;
  static constexpr int
    assetWidth = 77;              // SunAsset width (Used for spawn bounds)
  static constexpr float
    spawnInterval = 1.0f;         // Amount of seconds between each sun spawn

  static constexpr sf::Vector2f
    collectionSite = { 0, 0 };    // Position of sun collection site                                       
  //Shared Variables
  public:
  static Sun* sunArray[MAX_SIMULTANEOUS_SUN];    // Array containing all Sun objects
  static int sunBalance;                         // Amount of sun collected
  static int sunArrayCntr;                       // Index of first empty space in array
  static float spawnTimer;                       // Keeps track of time between last spawn
  static bool hovering;

  // Personal Functions (Related to current Sun struct)
  void collect();
  void draw();
  bool update(float dt); // Returns whether a sun is alive or not (deleted)
  bool onClick();        // Returns whether a sun is hovered or not

  // Public Functions (Related to all Sun structs)
  static void manageSuns(float dt, State s=Falling);  // Manages spawning , update , draw and hover
  static void generate(float x, float y, int val);
  static void spawn(int val=25);
  static void destroy(int idx);
};