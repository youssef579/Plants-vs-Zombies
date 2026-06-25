#pragma once

#include <SFML/Network.hpp>
#include <deque>

struct Peer {

  enum State {OffGame, Waiting, Requesting, Accepting, InGame};
  enum Command {Heartbeat, SpawnPlant, SpawnZombie, GameRequest, GameAccept};
  enum playerType {Plants, Zombies};
  enum Result {PlantsWon, ZombiesWon, Ongoing};

  struct Tick {
    int tickNumber = -1;
    Peer::Command cmd = Heartbeat;
    int row, col, type, cost;
  };

  static constexpr float timePerTick = 1.f / 90;
  static constexpr int tickDelay = 5;
  static constexpr int sendDelay = 1; // s
  static constexpr int outro = 500;
  static constexpr int gameTime = 300; // s
  static constexpr int patience = 500;

  float nextSendTimer = sendDelay;

  int currentTick = 0;
  int patienceTimer = 0;
  int outroTimer = 0;

  State state = OffGame;
  playerType type = Plants;

  bool initialized = false;

  Result matchResult = Ongoing;

  sf::UdpSocket socket;

  std::optional<sf::IpAddress> opponentIP;
  unsigned short opponentPort;
  unsigned short localPort;

  Tick arrBuffer[100] = {};
  Tick myArrBuffer[100] = {};
  Tick arrHistory[100] = {};

  std::map<int, Tick> buffer;
  std::map<int, Tick> myBuffer;
  std::deque<Tick> history;

  void init();
  void fillHistory();
  void connect(float dt);
  sf::Packet createPacket();
  void send(sf::Packet);
  void receive();
  void update();
  void apply(Tick&, bool mine);
  void spawnZombie(int row, int col, int type, int cost, bool mine);
  void spawnPlant(int row, int col, int type, int cost, bool mine);
  void exitMatch();
};

extern Peer peer;

extern Peer::Command CMD;
extern int ROW, COL, TYPE, COST;