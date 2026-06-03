#ifndef MONSTERFACTORY_H
#define MONSTERFACTORY_H

#include "GameDatabase.h"

class Monster;

class MonsterFactory {
 public:
  Monster* createMonster(const MonsterRecord& data) const;
};

#endif
