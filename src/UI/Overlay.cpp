#include <AssetsManager.hpp>
#include <UI/Overlay.hpp>
#include <Window.hpp>
#include <algorithm>
#include <globals.hpp>

Overlay *overlay;
bool isOverlayChanged = false;

void initOverlay() {
  sf::RectangleShape rect(sf::Vector2f{(float)WINDOW_SIZE.x * 4, (float)WINDOW_SIZE.y * 4});
  rect.setOrigin((sf::Vector2f)WINDOW_SIZE/2.0f);
  rect.move({ -(float)WINDOW_SIZE.x, -(float)WINDOW_SIZE.y });
  rect.setFillColor(sf::Color({0, 0, 0, 150}));

  int mediumSize = 40, largeSize = 60;
  auto createText = [&]() { return sf::Text(assets->font, "", mediumSize); };

  sf::Text title = createText(), greenButton = createText(),
           redButton = createText();

  title.setCharacterSize(largeSize);
  greenButton.setFillColor(sf::Color::Green);
  redButton.setFillColor(sf::Color::Red);

  overlay = new Overlay({0, rect, title, greenButton, redButton});
  for (int i = 0; i < MAX_LINES; i++)
    overlay->lines[i] = createText();
}

void placeOverlayContent() {
  sf::Vector2<float> greenButtonSize =
                         overlay->greenButton.getLocalBounds().size,
                     redButtonSize = overlay->redButton.getLocalBounds().size;

  float gapY = 60, titleExtraGap = 30, bias = 10,
        contentHeight = (overlay->nLines + 1) * gapY + titleExtraGap +
                        std::max(greenButtonSize.y, redButtonSize.y),
        offsetY = (WINDOW_SIZE.y - contentHeight) / 2 - bias;

  auto centerX = [&](sf::Text &text) {
    return (WINDOW_SIZE.x - text.getLocalBounds().size.x) / 2.f;
  };

  overlay->title.setPosition({centerX(overlay->title), offsetY});

  for (int i = 0; i < overlay->nLines; i++)
    overlay->lines[i]->setPosition({centerX(*overlay->lines[i]),
                                    (i + 1) * gapY + titleExtraGap + offsetY});

  overlay->greenButton.setPosition(
      {centerX(overlay->greenButton),
       (overlay->nLines + 1) * gapY + titleExtraGap + offsetY});
  overlay->redButton.setPosition(
      {centerX(overlay->redButton),
       (overlay->nLines + 1) * gapY + titleExtraGap + offsetY});

  if (!overlay->greenButton.getString().isEmpty() &&
      !overlay->redButton.getString().isEmpty()) {
    float gapX = 50,
          offsetX = (WINDOW_SIZE.x - (greenButtonSize.x + redButtonSize.x + gapX)) / 2;
    overlay->redButton.setPosition(
        {offsetX, (overlay->nLines + 1) * gapY + titleExtraGap + offsetY});
    overlay->greenButton.setPosition(
        {redButtonSize.x + gapX + offsetX,
         (overlay->nLines + 1) * gapY + titleExtraGap + offsetY});
  }
}

void setOverlayData(int nLines, std::string title,
                    std::function<void(sf::Text &, int)> setupLine,
                    std::string greenButtonText, std::string redButtonText) {
  overlay->nLines = nLines;

  overlay->title.setString(title);
  overlay->greenButton.setString(greenButtonText);
  overlay->redButton.setString(redButtonText);

  for (int i = 0; i < nLines; i++) {
    overlay->lines[i]->setFillColor(sf::Color::White);
    setupLine(*overlay->lines[i], i);
  }

  placeOverlayContent();
}

void handleOverlayEvents(std::function<void(sf::Text &, int)> updateLine,
                         std::function<void()> greenButtonAction,
                         std::function<void()> redButtonAction) {
  if (updateLine) {
    for (int i = 0; i < overlay->nLines; i++)
      updateLine(*overlay->lines[i], i);
  }

  if (greenButtonAction)
    onClick(overlay->greenButton, greenButtonAction);

  if (redButtonAction)
    onClick(overlay->redButton, redButtonAction);
}

void drawOverlay() {
  window->draw(overlay->overlayRect);
  window->draw(overlay->title);
  for (int i = 0; i < overlay->nLines; i++)
    window->draw(*overlay->lines[i]);

  if (!overlay->greenButton.getString().isEmpty())
    window->draw(overlay->greenButton);
  if (!overlay->redButton.getString().isEmpty())
    window->draw(overlay->redButton);
}
