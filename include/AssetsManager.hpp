#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

const int MAX_RESOURCES = 256;

template <typename T> struct Resource {
  T resource;
  std::string path;
};

struct AssetsManager {
  Resource<sf::Texture> *textures[MAX_RESOURCES]{};
  Resource<sf::SoundBuffer> *sounds[MAX_RESOURCES]{};
  sf::Font font;
};

extern AssetsManager *assets;

void throwLoadingError(std::string);

void initAssets();

sf::Texture &getTexture(std::string);

sf::SoundBuffer &getSound(std::string);
