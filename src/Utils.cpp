#include <Window.hpp>
#include <globals.hpp>
#include <random> //lw la2eto tari2a mn8er library feel free bs di azon standard library asln

void onClick(sf::Text& button, std::function<void()> action) {
  static bool wasButtonClicked = false;
  sf::Vector2f mousePosition =
    window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  if (button.getGlobalBounds().contains(mousePosition)) {
    button.setStyle(sf::Text::Bold);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!wasButtonClicked) {
        action();
        wasButtonClicked = true;
      }
    }
    else
      wasButtonClicked = false;

  }
  else
    button.setStyle(sf::Text::Regular);
}

bool onClickSun(Sun*& sun, std::function<void(Sun* s)> action) {
  static bool wasButtonClicked = false;
  static bool hovering = false;
  sf::Vector2f mousePosition =
    window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  if (sun->sprite.getGlobalBounds().contains(mousePosition)) {
    hovering = true;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!wasButtonClicked) {
        action(sun);
        wasButtonClicked = true;
      }
    }
    else
      wasButtonClicked = false;

   return true;

  }
  return false;

}


//Returns slider value ranged (0->100)
float updateSlider(Slider& slider) {
  static bool wasButtonClicked = false;
  sf::Vector2f mousePosition =
    window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  //7eta di m7taga some optimization probably bdl kol el getPosition()'s di
  if ((slider.sprite.getGlobalBounds().contains(mousePosition) || slider.isHolding) &&
    sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !wasButtonClicked) {
    slider.isHolding = true;
    wasButtonClicked = true;

    slider.sprite.setPosition({ mousePosition.x - 9.0f, slider.sprite.getPosition().y });
    if (slider.sprite.getPosition().x < slider.lowerBound) { //Check if before min position
      slider.sprite.setPosition({ slider.lowerBound, slider.sprite.getPosition().y });
    }
    else if (slider.sprite.getPosition().x > slider.upperBound) { //Check if after max position
      slider.sprite.setPosition({ slider.upperBound, slider.sprite.getPosition().y });
    }

  }
  else {
    slider.isHolding = false;
    wasButtonClicked = false;
  }
  //return value of slider according to ratio between its length and positions
  return ((slider.sprite.getPosition().x + 9.0f) / (slider.length)-(float)((slider.lowerBound + 9.0f) / slider.length)) * 100; //Holy Math!

}

float randomRange(float x, float y) { //random value from x to y
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(x, y);
  return dist(gen);
}