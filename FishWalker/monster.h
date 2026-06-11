#ifndef MONSTER_H
#define MONSTER_H

#include <string>

#include "character.h"

class Monster : public Character {
 public:
  Monster(const std::string& name, int id, int hp, int maxHp, int atk, int inf);
  virtual ~Monster();
  void attack(Character& target) override;

  void setPoisonTurns(int turns);
  void setStunned(bool stunned);
  void setAtkDebuff(int turns, int percent);
  void updateStatuses();
  std::string getStatusString() const;

  bool isStunned() const;
  int getAtkDebuffPercent() const;

 private:
  int poisonTurns = 0;
  bool stunned = false;
  int atkDebuffTurns = 0;
  int atkDebuffPercent = 0;
};

#endif
