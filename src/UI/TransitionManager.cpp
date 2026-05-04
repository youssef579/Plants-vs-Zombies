#include <UI/TransitionManager.hpp>
#include <cmath>
//#include <iostream> // REMOVE LATER
#include <globals.hpp>
#include <Game.hpp>

std::function<void()> TransitionManager::action;
float                 TransitionManager::timer;
bool                  TransitionManager::state;
bool                  TransitionManager::active;
float                 TransitionManager::maxRadius;
float                 TransitionManager::duration;
sf::CircleShape       TransitionManager::circle;


void TransitionManager::start(std::function<void()> _action) {
  // add logic to skip transition and call action when start is called while already active LATER
  action = _action;
  timer = 0;
  state = false;
  active = true;
  duration = 0.7f;
  //maxRadius = std::sqrt((WINDOW_SIZE.x*WINDOW_SIZE.x) + (WINDOW_SIZE.y*WINDOW_SIZE.y)) / 2.0f + 300.0f;
  /*sf::View defView = window->getDefaultView();
  sf::Vector2f center = defView.getCenter();
  sf::Vector2f size = defView.getSize();*/
  sf::Vector2f size{ (float)window->getSize().x, (float)window->getSize().y };
  sf::Vector2f center = size / 2.0f;
  maxRadius = std::sqrt((size.x * size.x) + (size.y * size.y)) / 2.0f + 100.0f;

  circle.setFillColor(sf::Color(0, 0, 0, 0));
  circle.setRadius(0.01f);
  circle.setOutlineColor(sf::Color::Black);
  circle.setOutlineThickness(maxRadius);
  circle.setOrigin(circle.getGeometricCenter());
  circle.setPosition(center);
  update(0.0f); // prevent snap at start of transition
}

void TransitionManager::update(float dt) {
  if (!active) return;
  if (!state)
    timer += dt / globalTimeModifier;
  else if (timer != duration) // prevent snap when heavy loading happens (big dt)
    timer -= dt / globalTimeModifier;
  else
    timer -= 0.0001f;

  //sf::Vector2f center = window->getDefaultView().getCenter();
  sf::Vector2f size{ (float)window->getSize().x, (float)window->getSize().y };
  sf::Vector2f center = size / 2.0f;

  if (timer >= duration) {
    timer = duration;
    state = true;
    circle.setRadius(0.0001f);
    circle.setOrigin({ 0.0001f, 0.0001f });
    //circle.setPosition((sf::Vector2f)WINDOW_SIZE / 2.0f);
    circle.setPosition(center);
    // draw early frame to prevent freeze on non-complete
    // transition (empty hole in middle)
    draw();
    window->display();
    action();
  }
  else if (timer <= 0 && state == true) {
    timer = 0;
    state = false;
    active = false;
  }
  //std::cout << "timer: " << timer << "\n";
  //std::cout << "state: " << state << "\n";
  float r = maxRadius * (1 - (timer / duration));
  if (r <= 0.0001f)
    r = 0.0001f;


  circle.setRadius(r);
  circle.setOrigin({ r, r });
  circle.setPosition(center);


}


//void TransitionManager::draw() {
//  sf::View prevView = window->getView();
//  window->setView(window->getDefaultView());
//  if(active)
//    window->draw(circle);
//  window->setView(prevView);
//}

void TransitionManager::draw() {
  if (active) {
    sf::View prevView = window->getView();


    sf::View absoluteView(sf::FloatRect({ 0.0f, 0.0f }, (sf::Vector2f)window->getSize()));

    window->setView(absoluteView);
    window->draw(circle);
    window->setView(prevView);
  }
}
