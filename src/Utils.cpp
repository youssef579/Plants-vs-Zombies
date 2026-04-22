#include <Window.hpp>
#include <globals.hpp>

void onClick(sf::Text &button, std::function<void()> action) {
  if (button.getGlobalBounds().contains(mousePosition)) {
    button.setStyle(sf::Text::Bold);

    if (isMousePressed)
      action();
  } else
    button.setStyle(sf::Text::Regular);
}

void onClick(sf::Sprite &button, std::function<void()> actionClick, std::function<void()> actionHover) {
  if (button.getGlobalBounds().contains(mousePosition)) {
    actionHover();
    if(isMousePressed)
      actionClick();
  }
}

void onClick(sf::FloatRect rect, float rot, std::function<void()> actionClick, std::function<void()> actionHover) {

  sf::FloatRect localRect({ 0, 0 }, rect.size);
  sf::Transform t;
  t.translate(rect.position);
  t.rotate(sf::degrees(rot));

  if (localRect.contains(t.getInverse().transformPoint(mousePosition))) {
    actionHover();
    if (isMousePressed)
      actionClick();
  }
}


int randomRange(int x, int y) {        // random int from x to y
  return x + rand() % (y - x + 1);
}

float randomRange(float x, float y) {  // random float from x to y
  return x + (rand()) / ( (float)(RAND_MAX / (y - x)));
}
