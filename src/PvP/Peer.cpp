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

void Peer::init() {
  socket.setBlocking(false);
  socket.bind(localPort);
}

void Peer::connect(float dt) {
  nextSendTimer += dt;
  if(nextSendTimer < sendDelay) return;
  nextSendTimer = 0;
  
  if(state == Requesting) CMD = GameRequest;
  else CMD = GameAccept;

  sf::Packet packet;
  packet << -1 << CMD;

  opponentIP = sf::IpAddress::Broadcast;
  opponentPort = (localPort == 53000 ? 54000 : 53000);
  
  send(packet);
  // std::cout << "Sent!" << '\n';
}

void Peer::fillHistory() {
  if(arrHistory[currentTick % 100].tickNumber == currentTick + tickDelay) return;
  Tick tick = {currentTick + tickDelay, CMD, ROW, COL, TYPE, COST};
  arrHistory[(currentTick + tickDelay) % 100] = tick;

  myArrBuffer[(currentTick + tickDelay) % 100] = tick;

  CMD = Heartbeat;
  ROW = COL = TYPE = -1;
}

sf::Packet Peer::createPacket() {
  sf::Packet packet;
  int size = std::min(20, currentTick - tickDelay - 1);
  if(size <= 0) {
    packet << 1 << currentTick << Heartbeat << 0 << 0 << 0 << 0;
    return packet;
  }
  packet << size;
  int currIdx = (currentTick) % 100;
  while(size--) {
    if(currIdx == -1) currIdx = 99;
    auto tick = arrHistory[currIdx];
    packet << tick.tickNumber << tick.cmd << tick.row << tick.col << tick.type << tick.cost;
    currIdx--;
  }
  return packet;
}

void Peer::send(sf::Packet packet) {
  // if(opponentIP.has_value()) std::cout << "Sending!" << '\n';
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
      if(!(packet >> size)) continue;

      if(size == -1) {
        int cmdInt;
        packet >> cmdInt;
        Command cmd = static_cast<Command>(cmdInt);
        // std::cout << "Receiving!" << '\n';
        if(state == Waiting && cmd == GameRequest) {
          opponentIP = ip;
          opponentPort = port;
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
        // std::cout << "Current: " << currentTick << '\n';
        // std::cout << "Size: " << history.size() << '\n';
        tick.cmd = static_cast<Command>(cmdInt);
        if(tick.tickNumber >= currentTick) {
          arrBuffer[tick.tickNumber % 100] = tick;
        }
        if(tick.tickNumber >= 0) state = InGame;
      }
    } else break;
  }
  if(state == InGame) update();
}

void Peer::update() {
  if(arrBuffer[currentTick % 100].tickNumber != currentTick && currentTick > tickDelay) {
    settings.timeModifier = 0;
    return;
  }
  settings.timeModifier = 1;
  
  Tick tick = arrBuffer[currentTick % 100];
  Tick myTick = myArrBuffer[currentTick % 100];

  if(tick.cmd == SpawnPlant || tick.cmd == SpawnZombie) apply(tick, false);
  if(myTick.cmd == SpawnPlant || myTick.cmd == SpawnZombie) apply(myTick, true);
  
  currentTick++;
}

void Peer::apply(Tick& tick, bool mine) {
  if(tick.cmd == SpawnZombie) spawnZombie(tick.row, tick.col, tick.type, tick.cost, mine);
  else spawnPlant(tick.row, tick.col, tick.type, tick.cost, mine);
}

void Peer::spawnPlant(int row, int col, int type, int cost, bool mine) {
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
  if(mine) Sun::sunBalance -= cost;
}

void Peer::spawnZombie(int row, int col, int type, int cost, bool mine) {
  Zombie::createZombie(
      grid[row][8].rectangle.getGlobalBounds().getCenter().x,
      grid[row][8].rectangle.getGlobalBounds().getCenter().y,
      static_cast<Zombie::Type>(type), row, 0);
  if(mine) Sun::sunBalance -= cost;
}