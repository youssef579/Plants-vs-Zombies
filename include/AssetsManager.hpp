#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

struct TextureResource {
  sf::Texture texture;
  std::string path;
};

struct AssetsManager {
  TextureResource *textures[256];
  sf::Font font;
};

extern AssetsManager *assets;

void throwLoadingError(std::string);

void initAssets();

sf::Texture &getTexture(std::string);
