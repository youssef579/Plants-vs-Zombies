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
  HoldLastFrame,
  LoopTimes // Loops a specific number of times
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
  int loopCount = 0;      // counts how many loops have passed
  int targetLoops = 0;

  bool remove = false;
};

struct PhysicsObject {
  sf::Vector2f position;
  sf::Vector2f velocity;
  sf::Vector2f acceleration;
  float rotation;
  float rotationSpeed;
  float groundY;
  float dissapearDuration = 0.0001f;
  float dissapearTimer = 0.0001f;
  sf::Sprite sprite;
  bool remove = false;

  void update(float dt);
  void draw(sf::RenderWindow *window);
};

static Array<PhysicsObject> physicsObjects;

struct ReAnimationDefinition {
  float fps;
  int frameCount=0;
  int totalTracks=0;
  Array<Label> labels;
  Array<Track> tracks;
  Array<sf::Transform> basePoses;  // constant
  std::unordered_map<std::string, sf::Texture *> textureMap;
  //std::unordered_map<std::string, Track *> trackMap;         // constant

  sf::FloatRect hitbox = sf::FloatRect({ 0, 0 }, { 0, 0 });
  sf::Vector2f offset = {0, 0}; // to change origin (top left by default)

  //std::unordered_map<std::string, Animation> animations;

  void loadFiles(std::string reAnimPath, int tracksNum, std::string tracks[],
    std::unordered_map<std::string, std::string> images);

  //void createTrackMap();

  sf::Vector2f getBasePose(Track &track);
  void calculateBasePoses();
  Transform getBaseTransform(Track &track);

  int getLabelIndex(std::string labelName);
  int getTrackIndex(std::string name);


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
  REANIM_CHERRYBOMB = 9,
  REANIM_ZOMBIE_CHARRED = 10,
  REANIM_JALAPENO = 11,
  REANIM_FIRE = 12,
  REANIM_LAWNMOWER = 13,
  REANIM_LAWNMOWERED_ZOMBIE = 14,
  REANIM_POTATO_MINE = 15,
  REANIM_ICESHROOM = 16,
  REANIM_SQUASH = 17,
  REANIM_ZOMBIE_SOCCER = 18
};

struct ReAnimator {

  ReAnimator *child = nullptr;
  int childsParentTrackIdx = -1;
  bool hasParent = false;

  ReAnimationDefinition *reAnimDef = nullptr;
  sf::RenderWindow *window;
  float x=0, y=0, sx=1, sy=1;
  float animSpeedMulti = 1.0f; // multiplier of animation speed
  float opacityMultiplier=1.0f;
  bool allowMotion = true;
  float motionMultiplier = 1.0f;
  sf::Color globalColor = { 255, 255, 255, 255 };


  sf::Transform rootMatrix;

  float timer=0.0f; // Global animTime
  Array<TrackInstance>     trackInstances;
  Array<Transform>         curTransforms;
  Array<bool>              curTransformsValid;
  Array<sf::Transform>     effectiveBasePoses;
  Array<bool>              effectiveBasePosesValid;
  Array<sf::Transform>     effectiveTransforms;
  Array<bool>              effectiveTransformsValid;
  Array<ActiveLabel>       activeLabels;








  void update(float dt);
  void draw();

  static float lerp(float a, float b, float t);
  static Transform lerpTransform(Transform a, Transform b, float t);


  bool updateLabel(ActiveLabel &lab);





  sf::Transform getEffectiveBasePose(int trackIndex);
  sf::Transform getEffectiveTransform(int trackIndex);
  static sf::Transform transformToSFML(Transform t);
  static int getFirstValidIdx(Track &track);
  sf::Transform getWorldTransform(int trackIdx);
  sf::Vector2f getWorldCenterPosition(int trackIdx);


  void playLabel(std::string labelName, LoopType loop = LoopType::Loop, float holdTimer = 0.0f);
  void playLabel(std::string labelName, LoopType loop, int loopCnt=0);
  void stopLabel(int labelIdx);

  void playAnimation(std::string labelName, LoopType loop = LoopType::Loop, float holdTimer = 0.0f);
  void playAnimation(std::string labelName, LoopType loop, int loopCnt);
  void stopAnimation(std::string labelName);
  //void replaceWithQueueAnimation(std::string labelNameA, std::string labelNameB);

  void setTrackVisibility(std::string trackName, bool newVisibility);
  void setTrackVisibility(Array<std::string> &trackNames, bool newVisibility);

  void forceSyncAll();

  static sf::Color multiplyColor(sf::Color color, float multiplier);


  void setScale(float SX, float SY);
  void setOpacity(uint8_t newOpacity);
  void move(sf::Vector2f offset);
  sf::Vector2f getPosition();
  void setPosition(sf::Vector2f newPos);
  sf::FloatRect getGlobalBounds();
  bool isPlayingAnimation(std::string animName="");
  void switchDefinition(ReAnimationDef newDefID);

  void setOverlayAlpha(float newAlpha);

  void separateTrackToPO(int trackIdx, sf::Vector2f velocity,
    sf::Vector2f acceleration, float groundY,
    float rotationSpeed, float dissapearDuration); // pop track into separate Physics Object


  static ReAnimationDefinition* getDefinition(ReAnimationDef defId);
  static void updateOrphans(float dt);
  static void drawOrphans();
  static void updatePhysicsObjects(float dt);
  static void drawPhysicsObjects(sf::RenderWindow *window);
  ReAnimator(ReAnimationDefinition *def, float x, float y, sf::RenderWindow *w);

  static Array<ReAnimator> orphanAnimators;

  // Debugging Functions
  void drawHitbox();
  void report();
};

void debugTransform(const sf::Transform &matrix);


void initReAnimDefs();
