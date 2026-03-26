#include <Window.hpp>
#include <globals.hpp>
#include <iostream> //REMOVE LATER

bool isHoldingSlider1 = false;




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

//float updateSlider(sf::Sprite* slider, float sliderLength, float lowerBound, float upperBound) {
//  sf::Vector2f mousePosition =
//    window->mapPixelToCoords(sf::Mouse::getPosition(*window));
//
//  //7eta di m7taga some optimization probably
//  if ((slider->getGlobalBounds().contains(mousePosition) || isHoldingSlider1) &&
//    sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
//    isHoldingSlider1 = true;
//    //
//    slider->setPosition({ mousePosition.x - 9.0f, slider->getPosition().y });
//    if (slider->getPosition().x < lowerBound) {
//      slider->setPosition({ lowerBound, slider->getPosition().y });
//    }
//    else if (slider->getPosition().x > upperBound) {
//      slider->setPosition({ upperBound, slider->getPosition().y });
//    }
//    //slider->setPosition({std::max(mousePosition.x - 9.0f, 0.0f)});
//  }
//  else isHoldingSlider1 = false;
//  return (slider->getPosition().x + 9.0f) / (sliderLength)-(float)((lowerBound + 9.0f) / sliderLength); //Holy Math!
//
//}



float updateSlider(Slider& slider) {
  static bool wasButtonClicked = false;
  sf::Vector2f mousePosition =
    window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  //7eta di m7taga some optimization probably
  if ((slider.sprite.getGlobalBounds().contains(mousePosition) || slider.isHolding) &&
    sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !wasButtonClicked) {
    slider.isHolding = true;
    wasButtonClicked = true;
    //
    slider.sprite.setPosition({ mousePosition.x - 9.0f, slider.sprite.getPosition().y });
    if (slider.sprite.getPosition().x < slider.lowerBound) {
      slider.sprite.setPosition({ slider.lowerBound, slider.sprite.getPosition().y });
    }
    else if (slider.sprite.getPosition().x > slider.upperBound) {
      slider.sprite.setPosition({ slider.upperBound, slider.sprite.getPosition().y });
    }
    //slider->setPosition({std::max(mousePosition.x - 9.0f, 0.0f)});
  }
  else {
    slider.isHolding = false;
    wasButtonClicked = false;
  }
  return (slider.sprite.getPosition().x + 9.0f) / (slider.length)-(float)((slider.lowerBound + 9.0f) / slider.length); //Holy Math!

}