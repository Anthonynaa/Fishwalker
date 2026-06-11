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
  bool isBattleOver() const;
  int countAliveEnemies() const;
  void basicAttack();
  void netAttack();
  void heavyAttack();
  void monstersAttack();
  bool isHeroAlive() const;

 private:
  Hero* hero;
  std::vector<Monster*> enemies;
  Monster* selectTarget() const;
};

#endif
