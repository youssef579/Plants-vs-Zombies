#include <AssetsManager.hpp>
#include <Overlay.hpp>
#include <Window.hpp>
#include <globals.hpp>

Overlay *overlay;
bool isOverlayChanged = false;

void initOverlay() {
  sf::RectangleShape rect((sf::Vector2f)window->getSize());
  rect.setFillColor(sf::Color({0, 0, 0, 200}));

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

void handleEvents(std::function<void()> &greenButtonAction,
                  std::function<void()> &redButtonAction) {
  if (!overlay->greenButton.getString().isEmpty())
    onClick(overlay->greenButton, greenButtonAction);
  if (!overlay->redButton.getString().isEmpty())
    onClick(overlay->redButton, redButtonAction);
}

void updateOverlay(int nLines, std::string lines[], std::string title,
                   std::function<void(sf::Text &)> handleLine,
                   std::function<void()> greenButtonAction,
                   std::string greenButtonText,
                   std::function<void()> redButtonAction,
                   std::string redButtonText) {
  if (isOverlayChanged) { // Update and place the content
    overlay->nLines = nLines;

    overlay->greenButton.setString(greenButtonText);
    overlay->redButton.setString(redButtonText);
    overlay->title.setString(title);

    sf::Vector2<float> greenButtonSize =
                           overlay->greenButton.getLocalBounds().size,
                       redButtonSize = overlay->redButton.getLocalBounds().size;
    sf::Vector2u windowSize = window->getSize();

    float gapY = 60, titleExtraGap = 30, bias = 10,
          contentHeight =
              (nLines + 1) * gapY + titleExtraGap + greenButtonSize.y,
          offsetY = (windowSize.y - contentHeight) / 2 - bias;

    auto centerX = [&](sf::Text &text) {
      return (windowSize.x - text.getLocalBounds().size.x) / 2.f;
    };

    overlay->title.setPosition({centerX(overlay->title), offsetY});

    for (int i = 0; i < nLines; i++) {
      overlay->lines[i]->setString(lines[i]);
      overlay->lines[i]->setPosition(
          {centerX(overlay->lines[i].value()),
           (i + 1) * gapY + titleExtraGap + offsetY});

      if (handleLine)
        handleLine(overlay->lines[i].value());
    }

    overlay->greenButton.setPosition(
        {centerX(overlay->greenButton),
         (nLines + 1) * gapY + titleExtraGap + offsetY});
    overlay->redButton.setPosition(
        {centerX(overlay->redButton),
         (nLines + 1) * gapY + titleExtraGap + offsetY});

    if (!greenButtonText.empty() && !redButtonText.empty()) {
      float gapX = 50,
            offsetX =
                (windowSize.x - (greenButtonSize.x + redButtonSize.x + gapX)) /
                2;
      overlay->greenButton.setPosition(
          {offsetX, (nLines + 1) * gapY + titleExtraGap + offsetY});
      overlay->redButton.setPosition(
          {greenButtonSize.x + gapX + offsetX,
           (nLines + 1) * gapY + titleExtraGap + offsetY});
    }
    isOverlayChanged = false;
  }

  handleEvents(greenButtonAction, redButtonAction);
}

void drawOverlay() {
  window->draw(overlay->overlayRect);
  window->draw(overlay->title);
  for (int i = 0; i < overlay->nLines; i++)
    window->draw(overlay->lines[i].value());

  if (!overlay->greenButton.getString().isEmpty())
    window->draw(overlay->greenButton);
  if (!overlay->redButton.getString().isEmpty())
    window->draw(overlay->redButton);
}
