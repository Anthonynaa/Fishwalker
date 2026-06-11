#include "monster.h"

#include <iostream>

Monster::Monster(const std::string& name, int id, int hp, int maxHp, int atk,
                 int inf)
    : Character(name, id, hp, maxHp, atk, inf) {}

Monster::~Monster() {}

void Monster::attack(Character& target) {
  if (stunned) {
    return;
  }
  int damage = getAtk();
  if (atkDebuffTurns > 0) {
    damage = damage * (100 - atkDebuffPercent) / 100;
  }
  if (damage < 0) damage = 0;
  target.takeDamage(damage);
}

void Monster::setPoisonTurns(int turns) { poisonTurns = turns; }
void Monster::setStunned(bool stunned) { this->stunned = stunned; }
void Monster::setAtkDebuff(int turns, int percent) {
  atkDebuffTurns = turns;
  atkDebuffPercent = percent;
}

void Monster::updateStatuses() {
  if (poisonTurns > 0) {
    takeDamage(5);
    std::cout << getName() << " suffers poison damage! HP left: " << getHp()
              << "\n";
    poisonTurns--;
  }
  if (stunned) {
    stunned = false;
  }
  if (atkDebuffTurns > 0) {
    atkDebuffTurns--;
    if (atkDebuffTurns == 0) atkDebuffPercent = 0;
  }
}

std::string Monster::getStatusString() const {
  std::string s;
  if (poisonTurns > 0) s += "[Poison]";
  if (stunned) s += "[Stunned]";
  if (atkDebuffTurns > 0)
    s += "[Atk- " + std::to_string(atkDebuffPercent) + "%]";
  return s;
}

bool Monster::isStunned() const { return stunned; }
int Monster::getAtkDebuffPercent() const { return atkDebuffPercent; }
