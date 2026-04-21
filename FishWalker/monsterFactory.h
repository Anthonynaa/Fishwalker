#ifndef MONSTERFACTORY_H
#define MONSTERFACTORY_H

#include "constants.h"
#include "infectedMonster.h"
#include "monster.h"

class MonsterFactory {
 public:
  Monster* createInfectedMonster() const;
  Monster* createNormalMonster() const;
};

#endif  // MONSTERFACTORY_H
