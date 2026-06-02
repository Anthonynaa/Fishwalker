#ifndef BATTLE_H
#define BATTLE_H

#include <vector>

#include "hero.h"
#include "monster.h"

class Battle {
 public:
  Battle(Hero* hero, std::vector<Monster*> enemies);
  ~Battle();

  Hero& getHero();
  Monster& getMonster();
  Monster* getMonsterPtr() const;

  bool isBattleOver() const;
  int countAliveEnemies() const;
  void heroAttack();
  void heroAttackWithMultiplier(int multiplier);
  bool isHeroAlive() const;

 private:
  Hero* hero;
  std::vector<Monster*> enemies;
};

#endif
