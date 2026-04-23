#include "monsterFactory.h"

Monster* MonsterFactory::createInfectedMonster() const {
  return new InfectedMonster(
      "Infected Fishwalker", 2, INFECTED_MONSTER_HP, INFECTED_MONSTER_MAX_HP,
      INFECTED_MONSTER_ATK, INFECTED_MONSTER_ACC, INFECTED_MONSTER_AGL,
      INFECTED_MONSTER_RES, INFECTED_MONSTER_INF, INFECTED_MONSTER_CRIT,
      INFECTED_MONSTER_INFECTION_POWER);
}
Monster* MonsterFactory::createNormalMonster() const {
  return new Monster("Normal Fishwalker", 1, NORMAL_MONSTER_HP,
                     NORMAL_MONSTER_MAX_HP, NORMAL_MONSTER_ATK,
                     NORMAL_MONSTER_ACC, NORMAL_MONSTER_AGL, NORMAL_MONSTER_RES,
                     NORMAL_MONSTER_INF, NORMAL_MONSTER_CRIT);
}
