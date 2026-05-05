#include <Packets/Packet.hpp>
#include <UI/Draw.hpp>
#include <Packets/Shovel.hpp>
#include <LevelProgress.hpp>
#include <PlantSelector.hpp>
#include <PvP/Peer.hpp>

void drawUI(float dt) {
  static bool runOnce = true;
  static sf::Text SunBalanceText(assets->font, std::to_string(Sun::sunBalance), 40);
  static sf::Texture& sunBankTexture = getTexture("assets/packets/seed_bank.png");
  static sf::Sprite sunBank(sunBankTexture);

  SunBalanceText.setString(std::to_string(Sun::sunBalance));
  SunBalanceText.setOrigin(SunBalanceText.getLocalBounds().getCenter()); //re-center text

  if (runOnce) {
    SunBalanceText.setPosition({ 45, 83 });
    SunBalanceText.setFillColor({ 0, 0, 0, 255 });
    SunBalanceText.setOutlineColor({ 255, 255, 255, 255 });
    SunBalanceText.setOutlineThickness(1.0f);
    SunBalanceText.setCharacterSize(20.0f);
    SunBalanceText.setStyle(sf::Text::Style::Regular);

    sunBank.setPosition({ 0, 0 });
    runOnce = false;
  }

  if(peer.state == Peer::InGame && peer.type == Peer::Zombies) {
    float shift = 1150 - sunBank.getGlobalBounds().size.x;
    SunBalanceText.setPosition({ 45 + shift, 83 });
    sunBank.setPosition({ shift, 0 });
  }

  // gameWeather.draw(*window);
  window->draw(sunBank); // Draw order matters
  window->draw(SunBalanceText);
  // for (int i = 0; i < packets.size; i++)
  //   packets[i].draw();
  // drawLevelProgress(dt);
  // plantSelector.drawSelector(*window);
}
