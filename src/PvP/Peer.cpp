#include <PvP/Peer.hpp>
#include <globals.hpp>
#include <Files.hpp>

#include <Plants/Plant.hpp>
#include <Zombies/Zombie.hpp>
#include <Grid.hpp>

void Peer::init(unsigned short port) {
  if(player == Plants) nextTick = 0;
  else nextTick = 1;
  socket.setBlocking(false);
  socket.bind(port);
}

void Peer::connect() {
  Command COMMAND;
  if(state == Requesting) COMMAND = GameRequest;
  else COMMAND = GameAccept;

  sf::Packet packet = createPacket(-5, COMMAND);
  opponentIP = sf::IpAddress::LocalHost; // Hardcode?
  
  send(packet);
}

sf::Packet Peer::createPacket(int TICK, Command COMMAND, int ROW, int COLUMN, int TYPE) {
  sf::Packet packet;
  packet << TICK << static_cast<int>(COMMAND) << ROW << COLUMN << TYPE;
  return packet;
}

void Peer::send(sf::Packet& packet) {
  socket.send(packet, opponentIP.value(), ports[plants^1]);
}

void Peer::receive() {
  sf::Packet packet;
  std::optional<sf::IpAddress> testIP;
  unsigned short testPort;

  while(socket.receive(packet, testIP, testPort) == sf::Socket::Status::Done) {
    int TICK, ROW, COLUMN, COMMAND, TYPE;
    packet >> TICK >> COMMAND >> ROW >> COLUMN >> TYPE;

    if(COMMAND == GameRequest) {
      if(state == Waiting) {
        opponentIP = testIP;
        state = Accepting;
      }
      continue;
    } else if(COMMAND == GameAccept) {
      if(state == Requesting) {
        state = InGame;
      }
      continue;
    }

    if(TICK == 1) state = InGame;
    
    // if(testIP != opponentIP) return;

    if((player == Plants && TICK == nextTick + 1) || (player == Zombies && TICK == nextTick)) {
      nextTick++;
      settings.timeModifier = 1;
    } else continue;
    
    update(static_cast<Command>(COMMAND), ROW, COLUMN, TYPE);
  }
}

void Peer::update(Command COMMAND, int ROW, int COLUMN, int TYPE) {
  if(COMMAND == Heartbeat) return;
  int opponent = player ^ 1;
  int i = ROW, j = COLUMN;
  if(opponent == Plants) {
    if(grid[i][j].plant.has_value()) return;
    switch (TYPE){
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
    
  } else {
    Zombie::createZombie(
      grid[i][8].rectangle.getGlobalBounds().getCenter().x,
      grid[i][8].rectangle.getGlobalBounds().getCenter().y,
      static_cast<Zombie::Type>(TYPE), i);
  }
}