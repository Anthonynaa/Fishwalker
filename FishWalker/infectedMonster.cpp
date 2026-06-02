#include "infectedMonster.h"

InfectedMonster::InfectedMonster(const std::string& name, int id, int hp,
                                 int maxHp, int atk, int inf,
                                 int infectionPower)
    : Monster(name, id, hp, maxHp, atk, inf), infectionPower(infectionPower) {}

InfectedMonster::~InfectedMonster() {}

void InfectedMonster::attack(Character& target) {
  if (!target.isAlive()) return;
  target.takeDamage(getAtk());
  if (target.isAlive()) target.takeInfection(infectionPower);
}

int InfectedMonster::getInfectionPower() const { return infectionPower; }
