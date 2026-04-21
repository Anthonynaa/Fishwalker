#include "monsterFactory.h"

Monster* MonsterFactory::createInfectedMonster() const {
  return new InfectedMonster("Infected Fishwalker", 2, INFECTED_MONSTER_HP,
                             INFECTED_MONSTER_MAX_HP, INFECTED_MONSTER_ATK, 50,
                             20, 10, 0, 5, INFECTED_MONSTER_INFECTION_POWER);
}
Monster* MonsterFactory::createNormalMonster() const {
  return new Monster("Normal Fishwalker", 1, NORMAL_MONSTER_HP,
                     NORMAL_MONSTER_MAX_HP, NORMAL_MONSTER_ATK, 50, 20, 10, 0,
                     5);
}
