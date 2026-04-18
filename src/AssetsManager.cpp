#include <AssetsManager.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

AssetsManager *assets;

void throwLoadingError(std::string path) {
  std::cerr << "FATAL ERROR: Could not load " << path << std::endl;
  std::cerr << "Check if the file exists in the correct folder!" << std::endl;
  std::system("pause");
  exit(1);
}

void initAssets() {
  assets = new AssetsManager();

  std::string fontPath = "assets/font.ttf";
  if (!assets->font.openFromFile(fontPath))
    throwLoadingError(fontPath);
}

sf::Texture &getTexture(std::string path) {
  for (int i = 0; i < MAX_RESOURCES; i++) {
    if (assets->textures[i].path.empty()) {
      if (!assets->textures[i].resource.loadFromFile(path))
        throwLoadingError(path);

      assets->textures[i].path = path;
      return assets->textures[i].resource;
    } else if (assets->textures[i].path == path)
      return assets->textures[i].resource;
  }

  std::cerr << "FATAL ERROR: Asset manager is full !" << std::endl;
  exit(1);
}

sf::SoundBuffer &getSoundBuffer(std::string path) {
  for (int i = 0; i < MAX_RESOURCES; i++) {
    if (assets->sounds[i].path.empty()) {
      if (!assets->sounds[i].resource.loadFromFile(path))
        throwLoadingError(path);

      assets->sounds[i].path = path;
      return assets->sounds[i].resource;
    } else if (assets->sounds[i].path == path)
      return assets->sounds[i].resource;
  }

  std::cerr << "FATAL ERROR: Asset manager is full !" << std::endl;
  exit(1);
}
