#include <Draw.hpp>


void drawUI() {
  static bool runOnce = true;
  static sf::Text SunBalanceText(assets->font, std::to_string(SunBalance), 40);
  static sf::Texture& sunBankTexture = getTexture("assets/Sun/sun_bank.png");
  static sf::Sprite sunBank(sunBankTexture);

  SunBalanceText.setString(std::to_string(SunBalance));
  SunBalanceText.setOrigin(SunBalanceText.getLocalBounds().getCenter()); //re-center text

  if (runOnce) {

    SunBalanceText.setPosition({ 37.0, 72.0 });
    SunBalanceText.setFillColor({ 0, 0, 0, 255 });
    SunBalanceText.setOutlineColor({ 255, 255, 255, 255 });
    SunBalanceText.setOutlineThickness(1.0f);
    SunBalanceText.setCharacterSize(20.0f);
    SunBalanceText.setStyle(sf::Text::Style::Regular);
    sunBank.setPosition({ 0, 0 });
    runOnce = false;
  }


  gameWeather.draw(*window);

  window->draw(sunBank); // Draw order matters !!
  window->draw(SunBalanceText);
}

void drawUISun() {
  drawSun();
}