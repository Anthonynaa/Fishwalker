#include "monsterFactory.h"

#include "infectedMonster.h"
#include "monster.h"

Monster* MonsterFactory::createMonster(const MonsterRecord& data) const {
  if (data.type == 1) {
    return new InfectedMonster(data.name, data.id, data.hp, data.maxHp,
                               data.atk, data.inf, data.infectionPower);
  }

  return new Monster(data.name, data.id, data.hp, data.maxHp, data.atk,
                     data.inf);
}
