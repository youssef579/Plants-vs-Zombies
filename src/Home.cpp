#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Files.hpp>
#include <Game.hpp>
#include <Home.hpp>
#include <UI/Overlay.hpp>
#include <Window.hpp>
#include <globals.hpp>

std::string names[] = {"Youssef Ragaey (Team Lead)",
                       "Anton Bakhet",
                       "Ali Assem",
                       "Mohammed Abdelhalim",
                       "Mohammed Ahmed",
                       "Ather Hossam",
                       "Mohammed Soliman"};

int homeState = 0;
/*
  0 -> Home menu
  1 -> Credits
  2 -> Level Selector
*/

void updateHome() {
  static sf::Texture &backgroundTexture = getTexture("assets/home.png");
  static sf::Texture &headerTexture = getTexture("assets/home_header.png");

  static sf::Sprite backgroundSprite(backgroundTexture);
  static sf::Sprite headerSprite(headerTexture);

  static sf::Text playButton(assets->font, "Play", 40);
  static sf::Text creditsButton(assets->font, "Credits", 40);
  static sf::Text exitButton(assets->font, "Exit", 40);

  static auto isLastPage = []() {
    return levelSelectorCurrentPage * LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE >=
           MAX_LEVELS;
  };
  static auto getLevelFromIndex = [](int i) {
    return (levelSelectorCurrentPage - 1) * LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE +
           1 + i;
  };
  static auto updateLevelsMenu = []() {
    setOverlayData(
        std::min(LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE,
                 MAX_LEVELS - (levelSelectorCurrentPage - 1) *
                                  LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE),
        "Select a Level",
        [](sf::Text &text, int i) {
          int currentLevel = getLevelFromIndex(i);
          text.setString("Level " + std::to_string(currentLevel));

          if (currentLevel > maxLevelUnlocked)
            text.setFillColor(sf::Color(120, 120, 120));
        },
        isLastPage() ? "" : "Next",
        levelSelectorCurrentPage == 1 ? "Back" : "Prev");
  };


  static bool runOnce = true;
  if (runOnce) {
    music.play("Menu");
    headerSprite.setPosition(
        {(window->getSize().x - headerTexture.getSize().x) / 2.0f, 20});

    playButton.setPosition({920, 420});
    creditsButton.setPosition({860, 470});
    exitButton.setPosition({920, 520});
    sf::Mouse::setPosition({0, 0}, *window);

    runOnce = false;
  }

  if (homeState == 0) {
    onClick(playButton, []() {
      homeState = 2;
      updateLevelsMenu();
    });
    onClick(creditsButton, []() {
      homeState = 1;
      setOverlayData(
          7, "Team Members",
          [](sf::Text &text, int i) { text.setString(names[i]); }, "Okay");
    });
    onClick(exitButton, []() { window->close(); });
  }

  if (homeState == 1)
    handleOverlayEvents(nullptr, []() { homeState = 0; });
  else if (homeState == 2) {
    handleOverlayEvents(
        [](sf::Text &text, int i) {
          int currentLevel = getLevelFromIndex(i);

          if (currentLevel <= maxLevelUnlocked)
            onClick(text, [&]() {
            music.play("DayStage");
              gameState = currentLevel;
            });
        },
        []() {
          if (!isLastPage()) {
            levelSelectorCurrentPage++;
            updateLevelsMenu();
          }
        },
        []() {
          if (levelSelectorCurrentPage == 1)
            homeState = 0;
          else {
            levelSelectorCurrentPage--;
            updateLevelsMenu();
          }
        });
  }

  window->draw(backgroundSprite);
  window->draw(headerSprite);
  window->draw(playButton);
  window->draw(creditsButton);
  window->draw(exitButton);

  if (homeState != 0)
    drawOverlay();
}
