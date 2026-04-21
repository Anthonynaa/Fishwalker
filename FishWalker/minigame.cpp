#include "Minigame.h"

#include <cmath>
#include <cstdlib>

Minigame::Minigame()
    : active(false),
      success(false),
      multiplier(1),
      barWidth(800.f),
      barHeight(80.f),
      targetPosition(0.f),
      currentPosition(0.f),
      speed(700.f),
      timeToStop(1.5f),
      stopped(false) {}

void Minigame::start(int accuracy) {
  active = true;
  success = false;
  stopped = false;
  multiplier = 1;
  float targetWidth = 120.f;
  float maxPos = barWidth - targetWidth;
  if (maxPos < 0) maxPos = 0;
  targetPosition = (rand() % (int)(maxPos + 1));
  currentPosition = 0.f;
  timeToStop = 1.5f;
}

void Minigame::update(float deltaTime) {
  if (!active) return;
  if (timeToStop > 0) {
    timeToStop -= deltaTime;
    // Если маркер достиг правого края (с учётом его ширины 40)
    if (currentPosition + 40.f >= barWidth && !stopped) {
      stop();  // вызываем stop, который выставит active = false и определит
               // результат
      return;
    }
    if (timeToStop <= 0) {
      stop();
      return;
    }
  }
  if (!stopped && timeToStop > 0) {
    currentPosition += speed * deltaTime;
    if (currentPosition + 40.f >= barWidth) {
      currentPosition = barWidth - 40.f;
      stop();
      return;
    }
  }
}

void Minigame::stop() {
  if (!active) return;
  stopped = true;
  float diff = std::abs(currentPosition - targetPosition);
  if (diff < 80.f) {
    success = true;
    multiplier = 2;
  } else if (diff < 120.f) {
    success = true;
    multiplier = 1;
  } else {
    success = false;
    multiplier = 0;
  }
  active = false;  // <-- сбрасываем активность, чтобы UI перестал рисовать
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

  sf::RectangleShape targetRect(sf::Vector2f(120.f, barHeight));
  targetRect.setFillColor(sf::Color(220, 50, 50));
  targetRect.setPosition(
      sf::Vector2f((window.getSize().x - barWidth) / 2 + targetPosition,
                   window.getSize().y / 2));
  window.draw(targetRect);

  sf::RectangleShape marker(sf::Vector2f(40.f, barHeight));
  marker.setFillColor(sf::Color(50, 220, 50));
  marker.setPosition(
      sf::Vector2f((window.getSize().x - barWidth) / 2 + currentPosition,
                   window.getSize().y / 2));
  window.draw(marker);

  static sf::Font font;
  static bool fontLoaded = false;
  if (!fontLoaded) {
    fontLoaded = font.openFromFile("arial.ttf");
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
