#ifndef MINIGAME_H
#define MINIGAME_H

#include <SFML/Graphics.hpp>

#include "constants.h"

struct MinigameConfig {
  float markerSpeed;
  float targetWidth;
  float timeLimit;
  float perfectZone;
  float goodZone;
};

namespace MinigamePresets {

static const MinigameConfig Attack{ATTACK_BAR_SPEED, ATTACK_TARGET_WIDTH,
                                   ATTACK_TIME_LIMIT, ATTACK_PERFECT_ZONE,
                                   ATTACK_GOOD_ZONE};

static const MinigameConfig Chest{CHEST_BAR_SPEED, CHEST_TARGET_WIDTH,
                                  CHEST_TIME_LIMIT, CHEST_PERFECT_ZONE,
                                  CHEST_GOOD_ZONE};
}  // namespace MinigamePresets

class Minigame {
 public:
  Minigame();
  void start(const MinigameConfig& config);
  void update(float deltaTime);
  void render(sf::RenderWindow& window);
  void stop();
  bool isActive() const;
  bool isSuccess() const;
  int getDamageMultiplier() const;

  void startChestMinigame();

  bool isMinigameRunning() const;

  bool isChestMinigameFinished() const;
  int getChestMinigameResult() const;

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

  float targetSize;
  float perfectZone;
  float goodZone;

  bool chestMinigameMode;
  bool chestMinigameFinished;
  int chestMinigameResult;
};

#endif
