#include <AssetsManager.hpp>
#include <Overlay.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <functional>
#include <globals.hpp>

Overlay *overlay;

void initOverlay() {
  sf::RectangleShape rect((sf::Vector2f)window->getSize());
  rect.setFillColor(sf::Color({0, 0, 0, 200}));

  int medium = 40, big = 55;

  overlay = new Overlay(
      {0,
       rect,
       sf::Text(assets->font, "", big),
       {sf::Text(assets->font, "", medium), sf::Text(assets->font, "", medium),
        sf::Text(assets->font, "", medium), sf::Text(assets->font, "", medium),
        sf::Text(assets->font, "", medium), sf::Text(assets->font, "", medium),
        sf::Text(assets->font, "", medium)},
       sf::Text(assets->font, "", medium),
       sf::Text(assets->font, "", medium)});

  overlay->greenButton.setFillColor(sf::Color::Green);
  overlay->redButton.setFillColor(sf::Color::Red);
}

void updateOverlay(int nLines, std::string lines[], std::string title,
                   std::function<void(sf::Text &)> handleLine,
                   std::function<void()> greenButtonHandle,
                   std::string greenButtonText,
                   std::function<void()> redButtonHandle,
                   std::string redButtonText) {
  overlay->nLines = nLines;
  overlay->greenButton.setString(greenButtonText);
  overlay->redButton.setString(redButtonText);
  sf::Vector2<float> greenButtonSize =
                         overlay->greenButton.getLocalBounds().size,
                     redButtonSize = overlay->redButton.getLocalBounds().size;

  float gapY = 60, titleExtraGap = 20, bias = 10,
        contentHeight = (nLines + 1) * gapY + titleExtraGap + greenButtonSize.y,
        offsetY = (window->getSize().y - contentHeight) / 2 - bias;

  auto centerX = [](sf::Text &text) {
    return (window->getSize().x - text.getLocalBounds().size.x) / 2.f;
  };

  overlay->title.setString(title);
  overlay->title.setPosition({centerX(overlay->title), offsetY});

  for (int i = 0; i < nLines; i++) {
    overlay->lines[i].setString(lines[i]);
    overlay->lines[i].setPosition(
        {centerX(overlay->lines[i]), (i + 1) * gapY + titleExtraGap + offsetY});

    if (handleLine)
      handleLine(overlay->lines[i]);
  }

  overlay->greenButton.setPosition(
      {centerX(overlay->greenButton),
       (nLines + 1) * gapY + titleExtraGap + offsetY});
  overlay->redButton.setPosition(
      {centerX(overlay->redButton),
       (nLines + 1) * gapY + titleExtraGap + offsetY});

  if (!greenButtonText.empty() && !redButtonText.empty()) {

    float gapX = 50, offsetX = (window->getSize().x -
                                (greenButtonSize.x + redButtonSize.x + gapX)) /
                               2;
    overlay->greenButton.setPosition(
        {offsetX, (nLines + 1) * gapY + titleExtraGap + offsetY});
    overlay->redButton.setPosition(
        {greenButtonSize.x + gapX + offsetX,
         (nLines + 1) * gapY + titleExtraGap + offsetY});
  }

  if (!greenButtonText.empty())
    onClick(overlay->greenButton, greenButtonHandle);
  if (!redButtonText.empty())
    onClick(overlay->redButton, greenButtonHandle);
}

void drawOverlay() {
  window->draw(overlay->overlayRect);
  window->draw(overlay->title);
  for (int i = 0; i < overlay->nLines; i++)
    window->draw(overlay->lines[i]);

  if (!overlay->greenButton.getString().isEmpty())
    window->draw(overlay->greenButton);
  if (!overlay->redButton.getString().isEmpty())
    window->draw(overlay->redButton);
}
