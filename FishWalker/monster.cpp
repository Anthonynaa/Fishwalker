#include "monster.h"

Monster::Monster(const std::string& name, int id, int hp, int maxHp, int atk,
                 int inf)
    : Character(name, id, hp, maxHp, atk, inf) {}

void Monster::attack(Character& target) { target.takeDamage(getAtk()); }

Monster::~Monster() {}
