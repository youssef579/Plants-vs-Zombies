#pragma once

#include <SFML/Network.hpp>

struct Peer {

  static constexpr unsigned short port = 53000;
  static constexpr float timerPerTick = 1.f / 20;
  static constexpr int tickShift = 2;
  static constexpr int PatienceLimit = 5;

  enum State {OffGame, Waiting, Requesting, Accepting, InGame};
  enum Player {Plants, Zombies};
  enum Command {Spawn, Heartbeat, GameRequest, GameAccept, Leaving};

  static sf::Packet createPacket(int TICK, Command COMMAND = Heartbeat, int ROW = -1, int COLUMN = -1, int TYPE = -1);

  std::optional<sf::IpAddress> opponentIP;

  sf::UdpSocket socket;

  State state = OffGame;
  Player player;

  int nextTick;
  int patience = 0;

  void init(unsigned short);
  void connect();
  void send(sf::Packet&);
  void update(Command COMMAND = Heartbeat, int ROW = -1, int COLUMN = -1, int TYPE = -1);
  void receive();
  
};

extern Peer peer;

extern Peer::Command command;
extern int row, column, type;