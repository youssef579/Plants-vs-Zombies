#pragma once

#include <SFML/Graphics.hpp>
#include <Array.hpp>
#include <AssetsManager.hpp>
#include <Plants/Plant.hpp>
#include <globals.hpp>

struct RewardManager {




  struct Reward {
    enum RewardType {Packet, SilverCoin, GoldCoin, DeckUpgrade};
    RewardType type;
    PlantType packetType; // if type is Packet
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float groundY;

    sf::Sprite sprite;
    bool remove = false;



    void update(float dt);
    void draw();

  };

  struct CollectedPacket {
    PlantType packetType;
    sf::Vector2f startPosition;
    sf::Vector2f endPosition;


    sf::Sprite *sprite;
    sf::RectangleShape whiteOverlay;

    float timer = 0;
    float duration = 3;
    sf::Vector2f startScale = {1, 1};
    sf::Vector2f endScale   = {3, 3};


    void update(float dt);
    void draw();

  };

  static Array<Reward> rewards;
  static CollectedPacket collectedPacket;
  static bool isPacketCollected;
  static bool spawnedLevelReward;

  // Shared Textures
  static sf::Texture packetTextures[];


  static void init();           // load textures
  static void update(float dt); // check clicks & update physics
  static void draw();

  static void spawnReward(sf::Vector2f pos, sf::Vector2f vInitial, Reward::RewardType type);
  static void spawnReward(sf::Vector2f pos, sf::Vector2f vInitial, PlantType type);
  static void collectPacket();


};
