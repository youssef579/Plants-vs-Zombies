#include <Animation.hpp>

void animateSpritesheet(Spritesheet& sheet, float dt) {
  sheet.timer -= dt;
  if (sheet.timer <= 0) {
    sf::IntRect rect = sheet.sprite->getTextureRect();
    rect.position.x = (rect.position.x + sheet.frameWidth)
      % (sheet.frameCount * sheet.frameWidth);
    sheet.sprite->setTextureRect(rect);
    sheet.timer = sheet.frameDuration;
  }
}