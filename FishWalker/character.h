#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

#include "entity.h"

class Character : public Entity {
 public:
  Character(const std::string& name, int id, int hp, int maxHp, int atk,
            int acc, int agl, int res, int inf, int crit);
  virtual ~Character();

  // Геттеры
  int getHp() const;
  int getMaxHp() const;
  int getAtk() const;
  int getAcc() const;
  int getAgl() const;
  int getRes() const;
  int getInf() const;
  int getCrit() const;

  // Методы изменения состояния
  bool isAlive() const;
  virtual void takeDamage(int amount);
  void heal(int amount);
  void takeInfection(int amount);

  // Виртуальные методы
  virtual void attack(Character& target) = 0;
  virtual void update() = 0;

  // Механика заражения
  void updateInfectionEffects();  // пересчёт характеристик от INF

 protected:
  // Сеттеры (защищённые)
  void setHp(int newHp);
  void setMaxHp(int newMaxHp);
  void setAtk(int newAtk);
  void setAcc(int newAcc);
  void setAgl(int newAgl);
  void setRes(int newRes);
  void setInf(int newInf);
  void setCrit(int newCrit);

  // Поля
  int hp, maxHp, atk, acc, agl, res, inf, crit;
  // Базовые значения (для восстановления после дебаффов)
  int baseAtk, baseAcc, baseAgl, baseRes, baseCrit, baseMaxHp;
};

#endif
