#include <AssetsManager.hpp>
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

    for (int i = 0; i < 256; i++)
      assets->textures[i] = nullptr;

    std::string fontPath = "assets/font.ttf";
    if (!assets->font.openFromFile(fontPath))
      throwLoadingError(fontPath);
  }
}

sf::Texture &getTexture(std::string path) {
  for (int i = 0; i < 256; i++) {
    if (assets->textures[i] == nullptr) {
      assets->textures[i] = new TextureResource();
      if (!assets->textures[i]->texture.loadFromFile(path))
        throwLoadingError(path);

      assets->textures[i]->path = path;
      return assets->textures[i]->texture;
    } else if (assets->textures[i]->path == path)
      return assets->textures[i]->texture;
  }

  std::cerr << "FATAL ERROR: Asset manager is full (256/256)!" << std::endl;
  exit(1);
}
