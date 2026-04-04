#include <UI/Checkbox.hpp>
#include <Window.hpp>

// Checks clicks for Checkbox and updates target
void updateCheckbox(Checkbox &checkbox, bool &target) {
  static bool wasButtonClicked = false;
  sf::Vector2f mousePosition =
      window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  if (checkbox.box.getGlobalBounds().contains(mousePosition)) {
    // button.setStyle(sf::Text::Bold);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!wasButtonClicked) {
        // action();
        checkbox.checked = !checkbox.checked;
        target = checkbox.checked;

        if (checkbox.checked)
          playSound("Tap1");
        else
          playSound("Tap2");

        wasButtonClicked = true;
      }
    } else
      wasButtonClicked = false;
  }
}
