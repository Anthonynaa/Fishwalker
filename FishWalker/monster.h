#ifndef MONSTER_H
#define MONSTER_H

#include <string>

#include "character.h"

class Monster : public Character {
 public:
  Monster(const std::string& name, int id, int hp, int maxHp, int atk, int inf);
  virtual ~Monster();
  void attack(Character& target) override;
};

#endif
