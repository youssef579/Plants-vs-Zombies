#pragma once

#include <iostream>
#include <unordered_map>
#include <set>
#include <SFML/Graphics.hpp>
#include <Array.hpp>

const int MAX_TRACKS = 60;


enum LoopType {
  PlayOnce,
  Loop,
  HoldLastFrame
};

struct Transform {
  float
    x, y,                 // Position
    sx, sy,               // Scale
    kx, ky,               // Skew
    f, a;                 // isActive and Alpha opacity
    //std::string i;      // image ID
  sf::Texture *i = nullptr;         // image pointer

};

//struct ActiveLabel;

struct Label {
  std::string name;
  int start, end;
  std::string next = ""; // next animation after death
};

struct Track {

  std::string name;
  std::vector<Transform> transforms;
  Track *parent = nullptr;
  sf::Vector2f basePose;    // base X, Y positions
  bool fullInherit = false; // Whether to inherit scale and skew from parent track

};

 //Used for overlay colors (eg. sunflower glow) & wallnut cracks
struct TrackInstance {
  sf::Color colorOverlay = sf::Color(255, 255, 255, 0);
  bool isVisible = true;

  //float colorOverlayIntensity = 0.0f;
  sf::Texture *imageOverride = nullptr;
};

//struct ReAnimationInstance {
//  std::vector<TrackInstance> trackInstances;
//};

//struct Animation {
//  std::vector<int> labels;
//  bool loop = false;         // Loops:  false = Play Once , true = Loop
//};

struct ActiveLabel {
  Label *label;
  LoopType loop = LoopType::PlayOnce;
  float offset = 0.0f;
  float holdTimer = 0.0f; // Time to hold after animation end
};

struct ReAnimationDefinition {
  float fps;
  int frameCount=0;
  int totalTracks=0;
  std::vector<Label> labels;
  std::vector<Track> tracks;
  std::unordered_map<std::string, sf::Texture *> textureMap;
  std::unordered_map<std::string, sf::Transform> basePoses;  // constant
  std::unordered_map<std::string, Track *> trackMap;         // constant

  sf::FloatRect hitbox = sf::FloatRect({ 0, 0 }, { 0, 0 });
  sf::Vector2f offset = {0, 0}; // to change origin (top left by default)

  //std::unordered_map<std::string, Animation> animations;

  void loadFiles(std::string reAnimPath, int tracksNum, std::string tracks[],
    std::unordered_map<std::string, std::string> images);

  void createTrackMap();

  sf::Vector2f getBasePose(Track &track);
  void calculateBasePoses();
  Transform getBaseTransform(Track &track);

  int getLabelIndex(std::string labelName);


  //void defineAnimation(std::string name, int labelCount, int labels[], bool loop = true);



};

enum ReAnimationDef {
  REANIM_SUN = 0,
  REANIM_SUNFLOWER = 1,
  REANIM_PEASHOOTER = 2,
  REANIM_WALLNUT = 3,
  REANIM_SNOWPEA = 4,
  REANIM_REPEATER = 5,
  REANIM_TALLNUT = 6,
  REANIM_ZOMBIE_BASIC = 7,
  REANIM_FLAGPOLE = 8,
  REANIM_CHERRYBOMB =9
};

struct ReAnimator {

  ReAnimator *child = nullptr;
  std::string childsParentTrack = "";
  bool hasParent = false;

  ReAnimationDefinition *reAnimDef = nullptr;
  sf::RenderWindow *window;
  float x=0, y=0, sx=1, sy=1;
  float animSpeedMulti = 1.0f; // multiplier of animation speed
  float opacityMultiplier=1.0f;
  bool allowMotion = true;
  sf::Color globalColor = { 255, 255, 255, 255 };


  sf::Transform rootMatrix;

  float timer=0.0f; // Global animTime
  std::vector<TrackInstance> trackInstances;



  std::unordered_map<std::string, sf::Transform> effectiveBasePoses;
  std::unordered_map<std::string, sf::Transform> effectiveTransforms;





  void update(float dt);

  static float lerp(float a, float b, float t);
  static Transform lerpTransform(Transform a, Transform b, float t);

  void draw();

  bool updateLabel(ActiveLabel lab);



  std::vector<ActiveLabel> activeLabels;

  std::unordered_map<std::string, Transform> curTransforms;

  sf::Transform getEffectiveBasePose(std::string trackName);
  sf::Transform getEffectiveTransform(std::string trackName);
  static sf::Transform transformToSFML(Transform t);
  static int getFirstValidIdx(Track &track);


  void playLabel(std::string labelName, LoopType loop = LoopType::Loop, float holdTimer=0.0f);
  void stopLabel(int labelIdx);

  void playAnimation(std::string labelName, LoopType loop = LoopType::Loop, float holdTimer=0.0f);
  void stopAnimation(std::string labelName);
  //void replaceWithQueueAnimation(std::string labelNameA, std::string labelNameB);

  void setTrackVisibility(std::string trackName, bool newVisibility);
  void setTrackVisibility(std::vector<std::string> trackNames, bool newVisibility);

  void report();
  void forceSyncAll();

  static sf::Color multiplyColor(sf::Color color, float multiplier);


  void setScale(float SX, float SY);
  void setOpacity(uint8_t newOpacity);
  void move(sf::Vector2f offset);
  sf::Vector2f getPosition();
  void setPosition(sf::Vector2f newPos);
  sf::FloatRect getGlobalBounds();
  bool isPlayingAnimation(std::string animName);

  void setOverlayAlpha(float newAlpha);

  void drawHitbox();

  static ReAnimationDefinition* getDefinition(ReAnimationDef defId);

  ReAnimator(ReAnimationDefinition *def, float x, float y, sf::RenderWindow *w);

};


void debugTransform(const sf::Transform &matrix);


void initReAnimDefs();

