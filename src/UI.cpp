#include <UI.hpp>



// Returns slider value ranged (0->100)
float updateSlider(Slider &slider) {
  static bool wasButtonClicked = false;
  sf::Vector2f mousePosition =
      window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  // 7eta di m7taga some optimization probably bdl kol el getPosition()'s di
  if ((slider.sprite.getGlobalBounds().contains(mousePosition) ||
       slider.isHolding) &&
      sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
      !wasButtonClicked) {
    slider.isHolding = true;
    wasButtonClicked = true;

    slider.sprite.setPosition(
        {mousePosition.x - 9.0f, slider.sprite.getPosition().y});
    if (slider.sprite.getPosition().x <
        slider.lowerBound) { // Check if before min position
      slider.sprite.setPosition(
          {slider.lowerBound, slider.sprite.getPosition().y});
    } else if (slider.sprite.getPosition().x >
               slider.upperBound) { // Check if after max position
      slider.sprite.setPosition(
          {slider.upperBound, slider.sprite.getPosition().y});
    }

  } else {
    slider.isHolding = false;
    wasButtonClicked = false;
  }
  // return value of slider according to ratio between its length and positions
  return ((slider.sprite.getPosition().x + 9.0f) / (slider.length) -
          (float)((slider.lowerBound + 9.0f) / slider.length)) *
         100; // Holy Math!
}

//Checks clicks for Checkbox and updates target
void updateCheckbox(Checkbox& cb, bool& target) {
  static bool wasButtonClicked = false;
  sf::Vector2f mousePosition =
    window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  if (cb.box.getGlobalBounds().contains(mousePosition)) {
    //button.setStyle(sf::Text::Bold);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!wasButtonClicked) {
        //action();
        cb.checked = !cb.checked;
        target = cb.checked;

        if (cb.checked) playSound("Tap1");
        else playSound("Tap2");

        wasButtonClicked = true;
      }
    }
    else
      wasButtonClicked = false;

  }
}