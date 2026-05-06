#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Files.hpp>
#include <Game.hpp>
#include <Home.hpp>
#include <UI/Overlay.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <cmath>
#include <LevelManager.hpp>
#include <UI/TransitionManager.hpp>

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

  /*static sf::Text playButton(assets->font, "Play", 40);
  static sf::Text optionsButton(assets->font, "Options", 40);
  static sf::Text creditsButton(assets->font, "Credits", 40);
  static sf::Text exitButton(assets->font, "Exit", 40);*/

  // Selector Screen
  static sf::Texture selectorScreenTombstoneT = getTexture("assets/SelectorScreen/SelectorScreen_Tombstone.png");
  static sf::Sprite selectorScreenTombstone(selectorScreenTombstoneT);

  static sf::Texture selectorScreenButton1T0 = getTexture("assets/SelectorScreen/SelectorScreen_1_Button0.png");
  static sf::Texture selectorScreenButton1T1 = getTexture("assets/SelectorScreen/SelectorScreen_1_Button1.png");
  static sf::Sprite selectorScreenButton1(selectorScreenButton1T0);

  static sf::Texture selectorScreenButton2T0 = getTexture("assets/SelectorScreen/SelectorScreen_2_Button0.png");
  static sf::Texture selectorScreenButton2T1 = getTexture("assets/SelectorScreen/SelectorScreen_2_Button1.png");
  static sf::Sprite selectorScreenButton2(selectorScreenButton2T0);

  static sf::Texture selectorScreenButton3T0 = getTexture("assets/SelectorScreen/SelectorScreen_3_Button0.png");
  static sf::Texture selectorScreenButton3T1 = getTexture("assets/SelectorScreen/SelectorScreen_3_Button1.png");
  static sf::Sprite selectorScreenButton3(selectorScreenButton3T0);

  static sf::Texture selectorScreenButton4T0 = getTexture("assets/SelectorScreen/SelectorScreen_4_Button0.png");
  static sf::Texture selectorScreenButton4T1 = getTexture("assets/SelectorScreen/SelectorScreen_4_Button1.png");
  static sf::Sprite selectorScreenButton4(selectorScreenButton4T0);

  static sf::Texture selectorScreenButtonOptionsT0 = getTexture("assets/SelectorScreen/SelectorScreen_Options0.png");
  static sf::Texture selectorScreenButtonOptionsT1 = getTexture("assets/SelectorScreen/SelectorScreen_Options1.png");
  static sf::Sprite selectorScreenButtonOptions(selectorScreenButtonOptionsT0);

  static sf::Texture selectorScreenButtonQuitT0 = getTexture("assets/SelectorScreen/SelectorScreen_Quit0.png");
  static sf::Texture selectorScreenButtonQuitT1 = getTexture("assets/SelectorScreen/SelectorScreen_Quit1.png");
  static sf::Sprite selectorScreenButtonQuit(selectorScreenButtonQuitT0);

  // Selector Screen button hitboxes
  static sf::FloatRect selectorScreenHitbox1 = {
    { (float)WINDOW_SIZE.x - 310.0f, (float)WINDOW_SIZE.y - 430.0f }, // pos
    {331 * 0.8f, 146 * 0.8f - 50} };  // size
  static sf::FloatRect selectorScreenHitbox2 = {
    { (float)WINDOW_SIZE.x - 302.0f, (float)WINDOW_SIZE.y - 353.0f }, // pos
    {331 * 0.72f, 146 * 0.75f - 50} };  // size
  static sf::FloatRect selectorScreenHitbox3 = {
    { (float)WINDOW_SIZE.x - 302.0f, (float)WINDOW_SIZE.y - 280.0f }, // pos
    {331 * 0.68f, 146 * 0.75f - 55} };  // size
  static sf::FloatRect selectorScreenHitbox4 = {
    { (float)WINDOW_SIZE.x - 304.0f, (float)WINDOW_SIZE.y - 220.0f }, // pos
    {331 * 0.63f, 146 * 0.75f - 60} };  // size


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

    /*playButton.setOrigin(playButton.getLocalBounds().size);
    optionsButton.setOrigin(optionsButton.getLocalBounds().size);
    creditsButton.setOrigin(creditsButton.getLocalBounds().size);
    exitButton.setOrigin(exitButton.getLocalBounds().size);*/

    selectorScreenTombstone.setOrigin(selectorScreenTombstone.getLocalBounds().size);
    selectorScreenTombstone.setScale({ 0.8f, 0.8f });
    selectorScreenTombstone.setPosition({(float)WINDOW_SIZE.x, (float)WINDOW_SIZE.y});

    selectorScreenButton1.setScale({0.8f, 0.8f});
    selectorScreenButton1.setPosition({ (float)WINDOW_SIZE.x - 315.0f, (float)WINDOW_SIZE.y - 440.0f });

    selectorScreenButton2.setScale({ 0.8f, 0.8f });
    selectorScreenButton2.setPosition({ (float)WINDOW_SIZE.x - 315.0f, (float)WINDOW_SIZE.y - 354.0f });

    selectorScreenButton3.setScale({ 0.8f, 0.8f });
    selectorScreenButton3.setPosition({ (float)WINDOW_SIZE.x - 312.0f, (float)WINDOW_SIZE.y - 280.0f });

    selectorScreenButton4.setScale({ 0.8f, 0.8f });
    selectorScreenButton4.setPosition({ (float)WINDOW_SIZE.x - 315.0f, (float)WINDOW_SIZE.y - 222.0f });

    selectorScreenButtonOptions.setScale({0.82f, 0.82f});
    selectorScreenButtonOptions.setPosition({ (float)WINDOW_SIZE.x - 192.0f, (float)WINDOW_SIZE.y - 95.0f });

    selectorScreenButtonQuit.setScale({ 0.92f, 0.92f });
    selectorScreenButtonQuit.setPosition({ (float)WINDOW_SIZE.x - 70.0f, (float)WINDOW_SIZE.y - 80.0f });

    /*playButton.setPosition({ 1000, 400 });
    optionsButton.setPosition({ 1000, 460 });
    creditsButton.setPosition({ 1000, 520 });
    exitButton.setPosition({ 1000, 580 });*/

    runOnce = false;
  }

  if (homeState == 0) {
    /*onClick(playButton, []() {
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

    onClick(exitButton, []() { window->close(); });*/


    selectorScreenButton1.setTexture(selectorScreenButton1T0);
    selectorScreenButton2.setTexture(selectorScreenButton2T0);
    selectorScreenButton3.setTexture(selectorScreenButton3T0);
    selectorScreenButton4.setTexture(selectorScreenButton4T0);
    selectorScreenButtonOptions.setTexture(selectorScreenButtonOptionsT0);
    selectorScreenButtonQuit.setTexture(selectorScreenButtonQuitT0);

    onClick(selectorScreenHitbox1, 8, []() {
      // Play
      sounds.play("Tap1");
      homeState = 2;
      updateLevelsMenu();
      }, []() {selectorScreenButton1.setTexture(selectorScreenButton1T1); });
    onClick(selectorScreenHitbox2, 10, []() {
      // Mini-games
      sounds.play("Tap1");
      }, []() {selectorScreenButton2.setTexture(selectorScreenButton2T1); });
    onClick(selectorScreenHitbox3, 10, []() {
      // Credits
      sounds.play("Tap1");
      homeState = 1;
      runOnceCredits = true;
      }, []() {selectorScreenButton3.setTexture(selectorScreenButton3T1); });
    onClick(selectorScreenHitbox4, 13, []() {
      // Button 4
      sounds.play("Tap1");
      }, []() {selectorScreenButton4.setTexture(selectorScreenButton4T1); });

    onClick(selectorScreenButtonOptions, []() {
      // Options
      homeState = 3;
      }, []() {selectorScreenButtonOptions.setTexture(selectorScreenButtonOptionsT1); });

    onClick(selectorScreenButtonQuit, []() {
      // Quit
      updateFiles(); // save data first
      window->close();
      }, []() {selectorScreenButtonQuit.setTexture(selectorScreenButtonQuitT1); });

  }

  window->draw(backgroundSprite);
  window->draw(headerSprite);

  window->draw(selectorScreenTombstone);
  window->draw(selectorScreenButton1);
  window->draw(selectorScreenButton2);
  window->draw(selectorScreenButton3);
  window->draw(selectorScreenButton4);
  window->draw(selectorScreenButtonOptions);
  window->draw(selectorScreenButtonQuit);

  //window->draw(playButton);
  //window->draw(optionsButton);
  //window->draw(creditsButton);
  //window->draw(exitButton);


  if (homeState == 1)
    updateCredits();
    //handleOverlayEvents(nullptr, []() { homeState = 0; });
  else if (homeState == 2) {
    handleOverlayEvents(
        [](sf::Text &text, int i) {
          int currentLevel = getLevelFromIndex(i);

          if (currentLevel <= maxLevelUnlocked)
            onClick(text, [=]() { // Play Level
              TransitionManager::start([=]() {
                levelManager.loadLevelData(currentLevel);
                music.play("DayStage");
                gameState = currentLevel;
                });


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
  static int state = 0; // 0 => bounce in , 1 => bounce out (runOnce) , 2 => bounce out

  static sf::Texture creditsNoteT = getTexture("assets/creditsNote4.png");
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
    if (state == 1) {
      sounds.play("Tap2");
      state = 2;
    }
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
