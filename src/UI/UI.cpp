#include <UI/UI.hpp>
#include <globals.hpp>


// Returns slider value ranged (0->100)
float updateSlider(Slider &slider) {
  static bool wasButtonClicked = false;
  static bool startedClickedInside = false;

  sf::Vector2f  pos = slider.sprite.getPosition();
  sf::FloatRect  bounds({ pos.x - 25.0f,pos.y - 25.0f }, { 30.0f,30.0f });

  if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) startedClickedInside = false;

  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    if ((bounds.contains(mousePosition) && !wasButtonClicked) || slider.isHolding) {
      startedClickedInside=true;
    }
  }

  // 7eta di m7taga some optimization probably bdl kol el getPosition()'s di
  if ((bounds.contains(mousePosition) || slider.isHolding) &&
      startedClickedInside &&
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

    if (isMousePressed) {
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

void drawTimeModifier(float dt) {
  static sf::Texture timeModifierT_1x = getTexture("assets/UI/TimeModifier/timeModifier_1x.png");
  static sf::Texture timeModifierT_2x = getTexture("assets/UI/TimeModifier/timeModifier_2x.png");
  static sf::Texture timeModifierT_3x = getTexture("assets/UI/TimeModifier/timeModifier_3x.png");
  static sf::SoundBuffer timeModifierSB_ding[] = {
    getSoundBuffer("assets/sounds/ding1.mp3"),
    getSoundBuffer("assets/sounds/ding2.mp3"),
    getSoundBuffer("assets/sounds/ding3.mp3")
  };
  //static sf::SoundBuffer timeModifierSB_ding2 = 
  //static sf::SoundBuffer timeModifierSB_ding3 = ;
  static sf::Sound timeModifier_sound(timeModifierSB_ding[0]);
  //static sf::Sprite  timeModifierS(timeModifierT_1x);
  static sf::Sprite timeModifierS = []() {
      sf::Sprite s(timeModifierT_1x);
      s.setOrigin({ s.getLocalBounds().size.x , 0});
      s.setPosition({ (float)WINDOW_SIZE.x + 4 , -46 });
      s.setScale({0.85f, 0.85f});
      return s;
    }();
  static PopupUI timeModifierPopup = []() {
      PopupUI timeModifierPopup;
      timeModifierPopup.init(&timeModifierS, timeModifierS.getPosition(), 46.0f, 120.0f, 1.0f);
      return timeModifierPopup;
    }();

  
  static int lastTimeModifier = 1;
  
  if (lastTimeModifier != settings.timeModifier) {
    switch (settings.timeModifier) {
    case 1: timeModifierS.setTexture(timeModifierT_1x); break;
    case 2: timeModifierS.setTexture(timeModifierT_2x); break;
    case 3: timeModifierS.setTexture(timeModifierT_3x); break;
    }
    lastTimeModifier = settings.timeModifier;

    timeModifierPopup.trigger();
    timeModifier_sound.setBuffer(timeModifierSB_ding[settings.timeModifier-1]);
    timeModifier_sound.setVolume(settings.soundFXVolume);
    timeModifier_sound.play();

  }



  timeModifierPopup.update(dt / settings.timeModifier); // to keep speed const
  if(timeModifierPopup.state != PopupUI::Idle)
    window->draw(timeModifierS);

}




void PopupUI::init(sf::Sprite *sp, sf::Vector2f startPos, float dropDistance, float moveSpeed, float wd) {
  sprite = sp;
  sprite->setPosition(startPos);
  startY = startPos.y;
  targetY = startPos.y + dropDistance;
  speed = moveSpeed;
  waitDuration = wd;
}

void PopupUI::trigger() {
  if (state == UIState::Idle || state == UIState::MovingUp) {
    state = UIState::MovingDown;
  }

  waitTimer.restart();
}

void PopupUI::update(float dt) {
  sf::Vector2f pos = sprite->getPosition();

  switch (state) {
  case UIState::MovingDown:
    pos.y += speed * dt;
    if (pos.y >= targetY) {
      pos.y = targetY;
      state = UIState::Waiting;
      waitTimer.restart();
    }
    break;

  case UIState::Waiting:
    if (waitTimer.getElapsedTime().asSeconds() >= waitDuration) {
      state = UIState::MovingUp;
    }
    break;

  case UIState::MovingUp:
    pos.y -= speed * dt;
    if (pos.y <= startY) {
      pos.y = startY;
      state = UIState::Idle;
    }
    break;

  //case UIState::Idle:
    //break;
  }

  sprite->setPosition(pos);
}
