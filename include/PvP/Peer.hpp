#pragma once

#include <SFML/Network.hpp>

#include <map>
#include <deque>

struct Peer {

  enum State {OffGame, Waiting, Requesting, Accepting, InGame};
  enum Command {Heartbeat, SpawnPlant, SpawnZombie, GameRequest, GameAccept};
  enum playerType {Plants, Zombies};

  struct Tick {
    int tickNumber;
    Peer::Command cmd;
    int row, col, type;
  };

  static constexpr float timePerTick = 1.f / 30;
  static constexpr int tickDelay = 5;
  static constexpr int sendDelay = 1; // ms

  float nextSendTimer = sendDelay;

  int currentTick = -1;

  State state = OffGame;
  playerType type;

  sf::UdpSocket socket;

  std::optional<sf::IpAddress> opponentIP;
  unsigned short opponentPort;
  unsigned short localPort;

  std::map<int, Tick> buffer;
  std::map<int, Tick> myBuffer;
  std::deque<Tick> history;

  void init();
  void fillHistory();
  void connect(float dt);
  sf::Packet createPacket();
  void send(sf::Packet&);
  void receive();
  void update();
  void apply(Tick&);
  void spawnZombie(int row, int col, int type);
  void spawnPlant(int row, int col, int type);

};

extern Peer peer;

extern Peer::Command CMD;
extern int ROW, COL, TYPE;