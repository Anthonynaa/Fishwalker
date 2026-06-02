#include "Minigame.h"

#include <cmath>
#include <cstdlib>

#include "constants.h"

Minigame::Minigame()
    : active(false),
      success(false),
      multiplier(1),
      barWidth(DEFAULT_BAR_WIDTH),
      barHeight(DEFAULT_BAR_HEIGHT),
      targetPosition(0.f),
      currentPosition(0.f),
      speed(DEFAULT_BAR_SPEED),
      timeToStop(DEFAULT_TIME_LIMIT),
      stopped(false),
      targetSize(DEFAULT_TARGET_SIZE),
      perfectZone(DEFAULT_PERFECT_ZONE),
      goodZone(DEFAULT_GOOD_ZONE) {}

void Minigame::start(const MinigameConfig& config) {
  active = true;
  success = false;
  stopped = false;

  multiplier = 1;

  speed = config.markerSpeed;
  timeToStop = config.timeLimit;

  barWidth = DEFAULT_BAR_WIDTH;

  currentPosition = 0.f;
  targetPosition = 0.f;

  float maxPos = barWidth - config.targetWidth;
  if (maxPos < 0) maxPos = 0;

  targetPosition = rand() % (int)(maxPos + 1);

  targetSize = config.targetWidth;
  perfectZone = config.perfectZone;
  goodZone = config.goodZone;
}

void Minigame::update(float deltaTime) {
  if (!active) return;
  if (timeToStop > 0) {
    timeToStop -= deltaTime;

    if (currentPosition + DEFAULT_MARKER_SIZE >= barWidth && !stopped) {
      stop();

      return;
    }
    if (timeToStop <= 0) {
      stop();
      return;
    }
  }
  if (!stopped && timeToStop > 0) {
    currentPosition += speed * deltaTime;
    if (currentPosition + DEFAULT_MARKER_SIZE >= barWidth) {
      currentPosition = barWidth - DEFAULT_MARKER_SIZE;
      stop();
      return;
    }
  }
}

void Minigame::stop() {
  if (!active) return;
  stopped = true;
  float diff = std::abs(currentPosition - targetPosition);
  if (diff < perfectZone) {
    success = true;
    multiplier = BOOSTED_MULTIPLIER;
  } else if (diff < goodZone) {
    success = true;
    multiplier = STANDARD_MULTIPLIER;
  } else {
    success = false;
    multiplier = 0;
  }
  active = false;
}

void Minigame::render(sf::RenderWindow& window) {
  if (!active) return;
  sf::RectangleShape overlay(
      sf::Vector2f(window.getSize().x, window.getSize().y));
  overlay.setFillColor(sf::Color(0, 0, 0, 200));
  window.draw(overlay);

  sf::RectangleShape bar(sf::Vector2f(barWidth, barHeight));
  bar.setFillColor(sf::Color(80, 80, 80));
  bar.setOutlineThickness(3.f);
  bar.setOutlineColor(sf::Color::White);
  bar.setPosition(sf::Vector2f((window.getSize().x - barWidth) / 2,
                               window.getSize().y / 2));
  window.draw(bar);

  sf::RectangleShape targetRect(sf::Vector2f(targetSize, barHeight));
  targetRect.setFillColor(sf::Color(220, 50, 50));
  targetRect.setPosition(
      sf::Vector2f((window.getSize().x - barWidth) / 2 + targetPosition,
                   window.getSize().y / 2));
  window.draw(targetRect);

  sf::RectangleShape marker(sf::Vector2f(DEFAULT_MARKER_SIZE, barHeight));
  marker.setFillColor(sf::Color(50, 220, 50));
  marker.setPosition(
      sf::Vector2f((window.getSize().x - barWidth) / 2 + currentPosition,
                   window.getSize().y / 2));
  window.draw(marker);

  static sf::Font font;
  static bool fontLoaded = false;
  if (!fontLoaded) {
    fontLoaded = font.openFromFile("supermario3dworld.otf");
  }
  if (fontLoaded) {
    sf::Text text(font);
    text.setCharacterSize(28);
    text.setFillColor(sf::Color::White);
    text.setString("Press SPACE or ENTER to stop the marker!");
    float textWidth = text.getLocalBounds().size.x;
    text.setPosition(sf::Vector2f((window.getSize().x - textWidth) / 2,
                                  window.getSize().y / 2 - 100.f));
    window.draw(text);
  }
}

bool Minigame::isActive() const { return active; }
bool Minigame::isSuccess() const { return success; }
int Minigame::getDamageMultiplier() const { return multiplier; }
