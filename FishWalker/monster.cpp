#include "monster.h"

#include <string>

Monster::Monster(const std::string& name, int id, int hp, int maxHp, int atk,
                 int acc, int agl, int res, int inf, int crit)
    : Character(name, id, hp, maxHp, atk, acc, agl, res, inf, crit) {}

void Monster::attack(Character& target) {
  int damage = getAtk();
  target.takeDamage(damage);
}

void Monster::update() {}

Monster::~Monster() {}
