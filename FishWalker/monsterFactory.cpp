#include "monsterFactory.h"

Monster* MonsterFactory::createInfectedMonster() const {
  return new InfectedMonster("Infected Fishwalker", INFECTED_MONSTER_ID,
                             INFECTED_MONSTER_HP, INFECTED_MONSTER_MAX_HP,
                             INFECTED_MONSTER_ATK, INFECTED_MONSTER_INF,
                             INFECTED_MONSTER_INFECTION_POWER);
}

Monster* MonsterFactory::createNormalMonster() const {
  return new Monster("Normal Fishwalker", NORMAL_MONSTER_ID, NORMAL_MONSTER_HP,
                     NORMAL_MONSTER_MAX_HP, NORMAL_MONSTER_ATK,
                     NORMAL_MONSTER_INF);
}
