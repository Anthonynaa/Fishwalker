#ifndef GAME_H
#define GAME_H

#include "GameDatabase.h"
#include "hero.h"
#include "itemFactory.h"
#include "monsterFactory.h"

class Game {
 public:
  Game();
  ~Game();

  void run();

 private:
  Hero hero;

  MonsterFactory monsterFactory;
  ItemFactory itemFactory;

  GameDatabase database;

  bool running;

  void showMainMenu();
  void showInventory();
  void startBattle(Monster* monster);
  void explore();
};

#endif
