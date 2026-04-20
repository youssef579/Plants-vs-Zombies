#include <UI/UI.hpp>
#include <globals.hpp>


// Returns slider value ranged (0->100)
float updateSlider(Slider &slider) {
  static bool wasButtonClicked = false;

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

//Checks clicks for Checkbox and updates target (returns true if value changed)
bool updateCheckbox(Checkbox& cb, bool& target) {
  static bool wasButtonClicked = false;

  cb.checked = target; // reset incase value was changed somewhere else

  if (cb.box.getGlobalBounds().contains(mousePosition)) {
    //button.setStyle(sf::Text::Bold);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!wasButtonClicked) {
        //action();
        cb.checked = !cb.checked;
        target = cb.checked;

        if (cb.checked) sounds.play("Tap1");
        else sounds.play("Tap2");

        wasButtonClicked = true;
        return true;
      }
    }
    else
      wasButtonClicked = false;

  }
  return false;
}

void drawTimeModifier() {
  static sf::Texture timeModifierT_1x = getTexture("assets/UI/TimeModifier/timeModifier_1x.png");
  static sf::Texture timeModifierT_2x = getTexture("assets/UI/TimeModifier/timeModifier_2x.png");
  static sf::Texture timeModifierT_3x = getTexture("assets/UI/TimeModifier/timeModifier_3x.png");
  //static sf::Sprite  timeModifierS(timeModifierT_1x);
  static sf::Sprite timeModifierS = []() {
    sf::Sprite s(timeModifierT_1x);
    s.setOrigin({ s.getLocalBounds().size.x , 0});
    s.setPosition({ (float)WINDOW_SIZE.x + 4 , 0 });
    s.setScale({0.85f, 0.85f});
    return s;
    }();

  static int lastTimeModifier = 1;
  
  if (lastTimeModifier != settings.timeModifier) {
    switch (settings.timeModifier) {
    case 1: timeModifierS.setTexture(timeModifierT_1x); break;
    case 2: timeModifierS.setTexture(timeModifierT_2x); break;
    case 3: timeModifierS.setTexture(timeModifierT_3x); break;
    }
    lastTimeModifier = settings.timeModifier;
  }

  window->draw(timeModifierS);

}
