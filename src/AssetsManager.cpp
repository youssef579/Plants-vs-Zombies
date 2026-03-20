#include <AssetsManager.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <iostream>

AssetsManager *assets = nullptr;

void throwLoadingError(std::string path) {
  std::cerr << "FATAL ERROR: Could not load " << path << std::endl;
  std::cerr << "Check if the file exists in the correct folder!" << std::endl;
  std::system("pause");
  exit(1);
}

void initAssets() {
  if (assets == nullptr) {
    assets = new AssetsManager();

    std::string fontPath = "assets/font.ttf";
    if (!assets->font.openFromFile(fontPath))
      throwLoadingError(fontPath);
  }
}

sf::Texture &getTexture(std::string path) {
  for (int i = 0; i < MAX_RESOURCES; i++) {
    if (assets->textures[i] == nullptr) {
      assets->textures[i] = new Resource<sf::Texture>();
      if (!assets->textures[i]->resource.loadFromFile(path))
        throwLoadingError(path);

      assets->textures[i]->path = path;
      return assets->textures[i]->resource;
    } else if (assets->textures[i]->path == path)
      return assets->textures[i]->resource;
  }

  std::cerr << "FATAL ERROR: Asset manager is full !" << std::endl;
  exit(1);
}

sf::SoundBuffer &getSound(std::string path) {
  for (int i = 0; i < MAX_RESOURCES; i++) {
    if (assets->sounds[i] == nullptr) {
      assets->sounds[i] = new Resource<sf::SoundBuffer>();
      if (!assets->sounds[i]->resource.loadFromFile(path))
        throwLoadingError(path);

      assets->sounds[i]->path = path;
      return assets->sounds[i]->resource;
    } else if (assets->sounds[i]->path == path)
      return assets->sounds[i]->resource;
  }

  std::cerr << "FATAL ERROR: Asset manager is full !" << std::endl;
  exit(1);
}
