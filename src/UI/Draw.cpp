#include <Packets/Packet.hpp>
#include <UI/Draw.hpp>
#include <Packets/Shovel.hpp>

#include <PvP/Peer.hpp>

void drawUI() {
  static bool runOnce = true;
  static sf::Text SunBalanceText(assets->font, std::to_string(Sun::sunBalance), 40);
  static sf::Texture& sunBankTexture = getTexture("assets/packets/seed_bank.png");
  static sf::Sprite sunBank(sunBankTexture);

  SunBalanceText.setString(std::to_string(Sun::sunBalance));
  SunBalanceText.setOrigin(SunBalanceText.getLocalBounds().getCenter()); //re-center text

  if (runOnce) {
    float shift = 0;
    if(peer.player == Peer::Zombies) shift = 1150 - sunBank.getGlobalBounds().size.x;
    SunBalanceText.setPosition({ 45 + shift, 83 });
    SunBalanceText.setFillColor({ 0, 0, 0, 255 });
    SunBalanceText.setOutlineColor({ 255, 255, 255, 255 });
    SunBalanceText.setOutlineThickness(1.0f);
    SunBalanceText.setCharacterSize(20.0f);
    SunBalanceText.setStyle(sf::Text::Style::Regular);

    sunBank.setPosition({ shift, 0 });

    for(int i = 0; i < zombiePackets.size; i++) {
      auto pos = zombiePackets[i].enabledSprite.getPosition();
      zombiePackets[i].disabledSprite.setPosition({pos.x + shift, pos.y});
      zombiePackets[i].enabledSprite.setPosition({pos.x + shift, pos.y});
    }

    runOnce = false;
  }


  gameWeather.draw(*window);
  window->draw(sunBank); // Draw order matters
  window->draw(SunBalanceText);
  // for (int i = 0; i < packets.size; i++)
  //   packets[i].draw();
}
