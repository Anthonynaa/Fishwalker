#ifndef BATTLEUI_H
#define BATTLEUI_H

#include <SFML/Graphics.hpp>

#include "battle.h"
#include "minigame.h"

class BattleUI {
 public:
  BattleUI();
  ~BattleUI();

  void init(sf::RenderWindow& window, Battle& battle);
  void handleEvent(const sf::Event& event);
  void update(float deltaTime);
  void render(sf::RenderWindow& window);

  bool isFinished() const;
  bool isChestMinigameFinished() const;
  int getChestMinigameResult() const;
  bool isMinigameRunning() const;
  void startChestMinigame();

 private:
  Battle* currentBattle;

  sf::Text* heroHpText;
  sf::Text* monsterHpText;
  sf::Text* actionMenuText;
  sf::Text* resultText;
  sf::Text* inventoryText;

  sf::RectangleShape heroHpBar;
  sf::RectangleShape monsterHpBar;
  sf::RectangleShape backgroundOverlay;

  bool battleFinished;
  bool showResult;
  bool showInventory;
  int selectedItemIndex;

  Minigame minigame;
  bool waitingForMinigame;

  bool chestMinigameMode;
  bool chestMinigameFinished;
  int chestMinigameResult;

  void updateBarsAndText();
  void showInventoryMenu();
  void useSelectedItem();
  void finishMinigame();
};

#endif
