#ifndef INFECTEDMONSTER_H
#define INFECTEDMONSTER_H

#include <string>

#include "monster.h"

class InfectedMonster : public Monster {
 public:
  InfectedMonster(const std::string& name, int id, int hp, int maxHp, int atk,
                  int inf, int infectionPower);
  ~InfectedMonster();
  void attack(Character& target) override;
  int getInfectionPower() const;

 private:
  int infectionPower;
};

#endif
