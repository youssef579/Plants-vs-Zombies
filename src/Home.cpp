#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Files.hpp>
#include <Game.hpp>
#include <Home.hpp>
#include <UI/Overlay.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <cmath>

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
  3 -> Options
*/

bool runOnceCredits;

void updateHome() {
  static sf::Texture &backgroundTexture = getTexture("assets/home.png");
  static sf::Texture &headerTexture = getTexture("assets/home_header.png");

  static sf::Sprite backgroundSprite(backgroundTexture);
  static sf::Sprite headerSprite(headerTexture);

  static sf::Text playButton(assets->font, "Play", 40);
  static sf::Text optionsButton(assets->font, "Options", 40);
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
    float scaleHeader = 0.5;
    // Scale main home images to fit fullscreen
    backgroundSprite.scale({(float)WINDOW_SIZE.x / backgroundTexture.getSize().x, (float)WINDOW_SIZE.y / backgroundTexture.getSize().y});
    headerSprite.scale({scaleHeader, scaleHeader});

    music.play("Menu");
    headerSprite.setPosition({(WINDOW_SIZE.x - headerTexture.getSize().x * scaleHeader) / 2.0f, 20});

    playButton.setOrigin(playButton.getLocalBounds().size);
    optionsButton.setOrigin(optionsButton.getLocalBounds().size);
    creditsButton.setOrigin(creditsButton.getLocalBounds().size);
    exitButton.setOrigin(exitButton.getLocalBounds().size);

    //playButton.setPosition({920, 420});
    ////optionsButton.setPosition({860, 470});
    //creditsButton.setPosition({860, 470});
    //exitButton.setPosition({920, 520});

    playButton.setPosition({ 1000, 400 });
    optionsButton.setPosition({ 1000, 460 });
    creditsButton.setPosition({ 1000, 520 });
    exitButton.setPosition({ 1000, 580 });

    runOnce = false;
  }

  if (homeState == 0) {
    onClick(playButton, []() {
      homeState = 2;
      updateLevelsMenu();
    });
    onClick(optionsButton, []() {
      homeState = 3;
      });
    onClick(creditsButton, []() {
        homeState = 1;
        runOnceCredits = true;
      });

    onClick(exitButton, []() { window->close(); });
  }

  window->draw(backgroundSprite);
  window->draw(headerSprite);
  window->draw(playButton);
  window->draw(optionsButton);
  window->draw(creditsButton);
  window->draw(exitButton);


  if (homeState == 1)
    updateCredits();
    //handleOverlayEvents(nullptr, []() { homeState = 0; });
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
  else if (homeState == 3) {
    pauseMenu.updateOptionsMenu();
    //pauseMenu.drawOptionsMenu();
  }

  

  if (homeState != 0 && homeState != 1 && homeState != 3)
    drawOverlay();
}


void updateCredits() {
  static bool state = false; // false => bounce in , true => bounce out

  static sf::Texture creditsNoteT = getTexture("assets/creditsNote3.png");
  static sf::Sprite creditsNote(creditsNoteT);
  static sf::Clock creditsClock;
  static float creditsTime;


  static sf::Texture creditsExitDisabledT = getTexture("assets/backButtonDisabled.png");
  static sf::Texture creditsExitEnabledT = getTexture("assets/backButtonEnabled.png");
  static sf::Sprite creditsExitButton(creditsExitDisabledT);
  static sf::Vector2f creditsFreezePos =
  { (float)WINDOW_SIZE.x / 2.0f,
    (float)(WINDOW_SIZE.y / 2.0f) * (float)std::cos(-4 * 1.0f)
      + ((float)WINDOW_SIZE.y / 2.0f) };
  if (runOnceCredits) {
    creditsNote.setPosition({ (float)WINDOW_SIZE.x / 2.0f, (float)WINDOW_SIZE.y });
    creditsExitButton.setOrigin(creditsExitButton.getLocalBounds().size / 2.0f);
    creditsExitButton.setScale({ 1.5f , 1.5f });
    creditsNote.setOrigin({ creditsNote.getLocalBounds().size.x/2.0f, 0 });
    creditsClock.start();
    runOnceCredits = false;
  }
  creditsTime = creditsClock.getElapsedTime().asSeconds();

  if (!state) {
    creditsTime *= 1.3f;
    if (creditsTime <= 1.0f)
      creditsNote.setPosition({ (float)WINDOW_SIZE.x / 2.0f, (float)(WINDOW_SIZE.y / 2.0f) * (float)std::cos(-4 * creditsTime) + (float)(WINDOW_SIZE.y / 2.0f) });
    else
      creditsNote.setPosition(creditsFreezePos);

    creditsExitButton.setTexture(creditsExitDisabledT);
    onClick(creditsExitButton, []() {
      creditsClock.restart();
      state = true;
      creditsExitButton.setTexture(creditsExitDisabledT);
      }, []() {creditsExitButton.setTexture(creditsExitEnabledT); });

  }
  else { // Bounce out
    creditsTime *= 1.4f;
    if(creditsTime <= 0.85753f)
      creditsNote.setPosition({ (float)WINDOW_SIZE.x / 2.0f, -(float)(WINDOW_SIZE.y / 2.0f) * (float)std::sin(4 * (creditsTime + 1.78484f + -0.0045)) + (float)(WINDOW_SIZE.y / 2.0f) });
    else { // Exit credits
      homeState = 0;
      creditsClock.reset();
      state = false;
    }
  }
  creditsExitButton.setPosition(creditsNote.getPosition() + sf::Vector2f(0, creditsNote.getLocalBounds().size.y - 25.0f));
  

  window->draw(creditsNote);
  window->draw(creditsExitButton);
}
