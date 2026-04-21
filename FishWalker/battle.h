#ifndef BATTLE_H
#define BATTLE_H

#include <vector>

#include "hero.h"
#include "monster.h"

class Battle {
 public:
  Battle(Hero* hero, std::vector<Monster*> enemies);
  ~Battle();

  void startBattle();
  bool isBattleOver() const;
  int countAliveEnemies() const;
  Hero& getHero();
  Monster& getMonster();
  void heroAttack();
  void heroAttackWithMultiplier(int multiplier);
  bool isHeroAlive() const;
  Monster* getMonsterPtr() const;

 private:
  Hero* hero;
  std::vector<Monster*> enemies;
  void monstersTurn();
  void heroTurn();
  void displayAliveEnemies();
};

#endif
