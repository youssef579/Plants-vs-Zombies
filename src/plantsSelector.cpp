#include <plantsSelector.hpp>

void selectionPacket::init(std::string name, float x, float y) {
  plantName = name;
  sprite = new sf::Sprite(getTexture("assets/myPackets/newSizes/" + name + ".png"));
  originalPos = { x, y };
  sprite->setPosition(originalPos);
}

void PlantSelector::setup() {
  background = new sf::Sprite(getTexture("assets/myPackets/choose.png"));
  background->setScale({ 0.68f, 0.64f });
  background->setPosition({ 0.f, -40.f });
  startButton = new sf::Sprite(getTexture("assets/myPackets/startPlantBtn.png"));
  startButton->setTextureRect(sf::IntRect({ 60, 680 }, { 250, 100 }));
  startButton->setPosition({ 60, 630 });
  //ather
  std::vector<std::string> names = {
      "cherryBomb", "jalapeno", "peaice", "peaShooter",
      "repeater", "sunflower", "wallnut", "tallnut"
  };

  float startX = 430.0f, startY = 220.0f;
  for (int i = 0; i < names.size(); ++i) {
    selectionPacket* p = new selectionPacket();
    float x = startX + (i % 4) * 115.0f;
    float y = startY + (i / 4) * 135.0f;
    p->init(names[i], x, y);
    inventory.push_back(p);
  }

  baskets.push_back(sf::FloatRect({ 105.f, 95.f }, { 75.f, 75.f }));
  baskets.push_back(sf::FloatRect({ 195.f, 95.f }, { 75.f, 75.f }));
  baskets.push_back(sf::FloatRect({ 285.f, 95.f }, { 75.f, 75.f }));
  baskets.push_back(sf::FloatRect({ 75.f, 230.f }, { 75.f, 75.f }));
  baskets.push_back(sf::FloatRect({ 150.f, 230.f }, { 75.f, 75.f }));
  baskets.push_back(sf::FloatRect({ 225.f, 230.f }, { 75.f, 75.f }));
  baskets.push_back(sf::FloatRect({ 300.f, 230.f }, { 75.f, 75.f }));
}


void PlantSelector::update(sf::RenderWindow& window) {
  sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    if (!draggedPacket) {
      for (auto* p : inventory) {
        if (p->sprite->getGlobalBounds().contains(mousePos) && !p->isChosen) {
          p->isDragging = true;
          draggedPacket = p;
          break;
        }
      }
    }
    else {
      draggedPacket->sprite->setPosition({ mousePos.x - 40.0f, mousePos.y - 50.0f });
    }
  }
  else if (draggedPacket) {
    bool placed = false;
    for (auto& b : baskets) {
      if (b.contains(mousePos) && selectedCount < 7) {
        draggedPacket->sprite->setPosition({ b.position.x + 5.f, b.position.y + 5.f });
        draggedPacket->isChosen = true;
        selectedCount++;
        placed = true;
        break;
      }
    }
    if (!placed) draggedPacket->sprite->setPosition(draggedPacket->originalPos);
    draggedPacket->isDragging = false;
    draggedPacket = nullptr;
  }
}

bool PlantSelector::isStartBtnPressed(sf::RenderWindow& window) {
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (startButton->getGlobalBounds().contains(mousePos) && selectedCount > 0) {
      return true;
    }
  }
  return false;
}

void PlantSelector::draw(sf::RenderWindow& window) {
  if (background) window.draw(*background);
  //for (auto* p : inventory) {
  //  if (p && p->sprite) window.draw(*(p->sprite));
  //}
}
