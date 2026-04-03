#ifndef WEATHER_SYSTEM_HPP
#define WEATHER_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

struct WeatherSystem {
  struct RainDrop {
    sf::RectangleShape shape;
    float speed;
  };

  sf::Sound* rainSound = nullptr;
  sf::Sound* thunderSound = nullptr;
  sf::SoundBuffer rainBuffer, thunderBuffer;
  sf::Clock timer;
  sf::Clock flashClock;
  sf::Clock rainClock;
  bool isRaining = false;
  float nextStrikeIn = 5.0f;
  bool isFlashing = false;
  sf::RectangleShape flashOverlay;
  std::vector<RainDrop> drops;

  WeatherSystem() {}

  ~WeatherSystem() {
    if (rainSound) delete rainSound;
    if (thunderSound) delete thunderSound;
  }

  void init(sf::Vector2u size) {
    if (rainBuffer.loadFromFile("assets/rain.wav")) {
      if (!rainSound) rainSound = new sf::Sound(rainBuffer);
      rainSound->setLooping(true);
      rainSound->setVolume(20.f);
    }
    if (thunderBuffer.loadFromFile("assets/thunder.wav")) {
      if (!thunderSound) thunderSound = new sf::Sound(thunderBuffer);
    }

    flashOverlay.setSize(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
    flashOverlay.setFillColor(sf::Color::Transparent);

    drops.clear();
    for (int i = 0; i < 150; i++) {
      RainDrop d;
      d.shape.setSize(sf::Vector2f(2.f, 15.f));
      d.shape.setFillColor(sf::Color(200, 200, 255, 100));
      d.shape.setPosition(sf::Vector2f(static_cast<float>(rand() % size.x), static_cast<float>(rand() % size.y)));
      d.speed = static_cast<float>((rand() % 400) + 600);
      drops.push_back(d);
    }
  }

  void update(sf::Vector2u size) {

    if (!isRaining) {
      if (rainSound && rainSound->getStatus() == sf::SoundSource::Status::Playing) {
        rainSound->stop();
      }
      return;
    }
    if (rainSound && rainSound->getStatus() != sf::SoundSource::Status::Playing) {
      rainSound->play();
    }

    float dt = rainClock.restart().asSeconds();


    for (auto& d : drops) {
      d.shape.move(sf::Vector2f(0.f, d.speed * dt));

      if (d.shape.getPosition().y > size.y) {
        d.shape.setPosition(sf::Vector2f(static_cast<float>(rand() % size.x), -20.f));
      }
    }

    if (timer.getElapsedTime().asSeconds() >= nextStrikeIn) {
      isFlashing = true;
      flashClock.restart();
      if (thunderSound) thunderSound->play();
      timer.restart();
      nextStrikeIn = static_cast<float>((rand() % 15) + 5);
    }

    if (isFlashing) {
      if (flashClock.getElapsedTime().asSeconds() < 0.1f)
        flashOverlay.setFillColor(sf::Color(255, 255, 255, 100));
      else {
        flashOverlay.setFillColor(sf::Color::Transparent);
        isFlashing = false;
      }
    }
  }

  void draw(sf::RenderWindow& targetWindow) {
    if (!isRaining) return;
    for (auto& d : drops) {
      targetWindow.draw(d.shape);
    }
    if (isFlashing) {
      targetWindow.draw(flashOverlay);
    }
  }
};

#endif