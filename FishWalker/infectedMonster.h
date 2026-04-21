#ifndef INFECTEDMONSTER_H
#define INFECTEDMONSTER_H

#include <string>

#include "monster.h"

class InfectedMonster : public Monster {
 public:
  // десруктор
  ~InfectedMonster();

  // конструктор
  InfectedMonster(const std::string& name, int id, int hp, int maxHp, int atk,
                  int acc, int agl, int res, int inf, int crit,
                  int infectionPower);

  // функция
  int getInfectionPower() const;

  void attack(Character& target) override;

  void update() override;

 private:
  int infectionPower;
};

#endif  // INFECTEDMONSTER_H
