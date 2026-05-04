#include <PvP/Peer.hpp>
#include <Files.hpp>
#include <globals.hpp>
#include <iostream>
#include <Plants/Plant.hpp>
#include <Grid.hpp>
#include <Zombies/Zombie.hpp>

Peer::Command CMD = Peer::Heartbeat;
int ROW = -1, COL = -1, TYPE = -1;

short unsigned PORT[2] = {53000, 53001};

void Peer::init() {
  socket.setBlocking(false);
  socket.bind(PORT[type]);
}

void Peer::connect(float dt) {
  nextSendTimer += dt;
  if(nextSendTimer < sendDelay) return;
  nextSendTimer = 0;
  
  if(type == Zombies) CMD = GameRequest;
  else CMD = GameAccept;

  sf::Packet packet;
  packet << -1 << CMD;

  opponentIP = sf::IpAddress::LocalHost;
  opponentPort = PORT[type ^ 1];
  
  send(packet);
  // std::cout << "Sent!" << '\n';
}

void Peer::fillHistory() {
  if(!history.empty() && history.back().tickNumber == currentTick + tickDelay) return;
  Tick tick = {currentTick + tickDelay, CMD, ROW, COL, TYPE};
  history.push_back(tick);

  if(history.size() > 20) {
    history.pop_front();
  }

  myBuffer[currentTick + tickDelay] = tick;

  CMD = Heartbeat;
  ROW = COL = TYPE = -1;
}

sf::Packet Peer::createPacket() {
  sf::Packet packet;
  int size = history.size();
  packet << size;
  for(auto& tick : history) {
    packet << tick.tickNumber << tick.cmd << tick.row << tick.col << tick.type;
  }
  return packet;
}

void Peer::send(sf::Packet& packet) {
  socket.send(packet, opponentIP.value(), opponentPort);
}

void Peer::receive() {
  sf::Packet packet;
  std::optional<sf::IpAddress> ip;
  unsigned short port;

  while(true) {
    packet.clear();
    if(socket.receive(packet, ip, port) == sf::Socket::Status::Done) {
      int size;
      packet >> size;

      if(size == -1) {
        int cmdInt;
        packet >> cmdInt;
        Command cmd = static_cast<Command>(cmdInt);
        // std::cout << "Receiving!" << '\n';
        if(state == Waiting && cmd == GameRequest) {
          // opponentIP = ip;
          // opponentPort = port;
          state = Accepting;
        } else if(state == Requesting && cmd == GameAccept) {
          state = InGame;
        }
        continue;
      }

      Tick tick;
      while(size--) {
        int cmdInt;
        packet >> tick.tickNumber >> cmdInt >> tick.row >> tick.col >> tick.type;
        // std::cout << "HERE?" << " " << tick.tickNumber << '\n';
        // std::cout << "Current: " << currentTick << '\n';
        // std::cout << "Size: " << history.size() << '\n';
        tick.cmd = static_cast<Command>(cmdInt);
        if(tick.tickNumber >= currentTick) {
          buffer[tick.tickNumber] = tick;
        }
        if(tick.tickNumber >= 0) state = InGame;
      }
    } else break;
  }
  if(state == InGame) update();
}

void Peer::update() {
  if(!buffer.count(currentTick) && currentTick >= tickDelay) {
    settings.timeModifier = 0;
    return;
  }
  settings.timeModifier = 1;
  
  Tick tick = buffer[currentTick];
  Tick myTick = myBuffer[currentTick];

  if(tick.cmd == SpawnPlant || tick.cmd == SpawnZombie) apply(tick);
  if(myTick.cmd == SpawnPlant || myTick.cmd == SpawnZombie) apply(myTick);

  buffer.erase(currentTick);
  myBuffer.erase(currentTick);
  
  currentTick++;
}

void Peer::apply(Tick& tick) {
  if(tick.cmd == SpawnZombie) spawnZombie(tick.row, tick.col, tick.type);
  else spawnPlant(tick.row, tick.col, tick.type);
}

void Peer::spawnPlant(int row, int col, int type) {
  int i = row, j = col;
  switch (type){
    case SUN_FLOWER:
      grid[i][j].plant = Plant(SUN_FLOWER, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_SUNFLOWER));
      break;
    case WALLNUT:
      grid[i][j].plant = Plant(WALLNUT, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_WALLNUT));
      break;
    case PEASHOOTER:
      grid[i][j].plant = Plant(PEASHOOTER, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_PEASHOOTER));
      break;
    case SNOWPEASHOOTER:
      grid[i][j].plant = Plant(SNOWPEASHOOTER, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_SNOWPEA));
      break;
    case REPEATERPEA:
      grid[i][j].plant = Plant(REPEATERPEA, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_REPEATER));
      break;
    case TALLNUT:
      grid[i][j].plant = Plant(TALLNUT, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_TALLNUT));
      break;
    default:
      break;
  }
  grid[i][j].therePlantInBounders = 0;
}

void Peer::spawnZombie(int row, int col, int type) {
  Zombie::createZombie(
      grid[row][8].rectangle.getGlobalBounds().getCenter().x,
      grid[row][8].rectangle.getGlobalBounds().getCenter().y,
      static_cast<Zombie::Type>(type), row);
}