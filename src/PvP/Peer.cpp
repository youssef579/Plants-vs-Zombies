#include <PvP/Peer.hpp>
#include <Files.hpp>
#include <globals.hpp>
#include <iostream>
#include <Plants/Plant.hpp>
#include <Grid.hpp>
#include <Zombies/Zombie.hpp>
#include <SunManager.hpp>

Peer::Command CMD = Peer::Heartbeat;
int ROW = -1, COL = -1, TYPE = -1, COST = -1;

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
  std::cout << "Sent!" << '\n';
}

void Peer::fillHistory() {
  if(!history.empty() && history.back().tickNumber == currentTick + tickDelay) return;
  Tick tick = {currentTick + tickDelay, CMD, ROW, COL, TYPE, COST};
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
    packet << tick.tickNumber << tick.cmd << tick.row << tick.col << tick.type << tick.cost;
  }
  return packet;
}

void Peer::send(sf::Packet packet) {
  // if(opponentIP.has_value()) std::cout << "Sending!" << '\n';
  // else std::cout << "A7A" << '\n';
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
        packet >> tick.tickNumber >> cmdInt >> tick.row >> tick.col >> tick.type >> tick.cost;
        // std::cout << "Have: " << " " << tick.tickNumber << '\n';
        std::cout << "Current: " << currentTick << '\n';
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
  if(!buffer.count(currentTick) && currentTick > tickDelay) {
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
  if(tick.cmd == SpawnZombie) spawnZombie(tick.row, tick.col, tick.type, tick.cost);
  else spawnPlant(tick.row, tick.col, tick.type, tick.cost);
}

void Peer::spawnPlant(int row, int col, int type, int cost) {
  int i = row, j = col;
  switch (type){
    case SUN_FLOWER:
      grid[i][j].plant = Plant(SUN_FLOWER, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_SUNFLOWER));
      break;
    case WALLNUT:
      grid[i][j].plant = Plant(WALLNUT, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_WALLNUT));
      break;
    case PEASHOOTER:
      grid[i][j].plant = Plant(PEASHOOTER, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_PEASHOOTER));
      break;
    default:
      break;
  }
  grid[i][j].therePlantInBounders = 0;
  sounds.play((rand() & 1) ? "Plant1" : "Plant2");
  Sun::sunBalance -= cost;
}

void Peer::spawnZombie(int row, int col, int type, int cost) {
  Zombie::createZombie(
      grid[row][8].rectangle.getGlobalBounds().getCenter().x,
      grid[row][8].rectangle.getGlobalBounds().getCenter().y,
      static_cast<Zombie::Type>(type), row, 0);
  Sun::sunBalance -= cost;
}