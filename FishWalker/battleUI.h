#ifndef BATTLEUI_H
#define BATTLEUI_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "battle.h"
#include "minigame.h"

class BattleUI {
 public:
  BattleUI();
  void init(sf::RenderWindow& window, Battle& battle);
  void handleEvent(const sf::Event& event);
  void update(float deltaTime);
  void render(sf::RenderWindow& window);
  bool isFinished() const;

 private:
  Battle* currentBattle;
  sf::Font font;
  std::unique_ptr<sf::Text> heroHpText;
  std::unique_ptr<sf::Text> monsterHpText;
  std::unique_ptr<sf::Text> actionMenuText;
  std::unique_ptr<sf::Text> resultText;
  std::unique_ptr<sf::Text> inventoryText;
  sf::RectangleShape heroHpBar;
  sf::RectangleShape monsterHpBar;
  sf::RectangleShape backgroundOverlay;
  bool battleFinished;
  bool showResult;
  bool showInventory;
  int selectedItemIndex;
  Minigame minigame;        // <-- добавлено
  bool waitingForMinigame;  // <-- добавлено

  void updateBarsAndText();
  void showInventoryMenu();
  void useSelectedItem();
  void finishMinigame();  // <-- добавлено
};

#endif
