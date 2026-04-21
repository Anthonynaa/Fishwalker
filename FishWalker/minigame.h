#ifndef MINIGAME_H
#define MINIGAME_H

#include <SFML/Graphics.hpp>

class Minigame {
 public:
  Minigame();
  void start(int accuracy);
  void update(float deltaTime);
  void render(sf::RenderWindow& window);
  void stop();  // принудительная остановка
  bool isActive() const;
  bool isSuccess() const;
  int getDamageMultiplier() const;

 private:
  bool active;
  bool success;
  int multiplier;
  float barWidth;
  float barHeight;
  float targetPosition;
  float currentPosition;
  float speed;
  float timeToStop;
  bool stopped;
};

#endif
