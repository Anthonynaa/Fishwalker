#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

class Character {
 public:
  Character(const std::string& name, int id, int hp, int maxHp, int atk,
            int inf);
  virtual ~Character();

  int getHp() const;
  int getMaxHp() const;
  int getAtk() const;
  int getInf() const;
  const std::string& getName() const;
  int getId() const;

  bool isAlive() const;
  virtual void takeDamage(int amount);
  void heal(int amount);
  void takeInfection(int amount);

  virtual void attack(Character& target) = 0;
  void updateInfectionEffects();

  void setHp(int newHp);
  void setMaxHp(int newMaxHp);
  void setAtk(int newAtk);
  void setInf(int newInf);
  void addMaxHp(int bonus);

 protected:
  std::string name;
  int id;
  int hp, maxHp, atk, inf;
  int baseAtk, baseMaxHp;
};

#endif
