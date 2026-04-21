#include "character.h"

Character::Character(const std::string& name, int id, int hp, int maxHp,
                     int atk, int acc, int agl, int res, int inf, int crit)
    : Entity(name, id),
      hp(hp),
      maxHp(maxHp),
      atk(atk),
      acc(acc),
      agl(agl),
      res(res),
      inf(inf),
      crit(crit),
      baseAtk(atk),
      baseAcc(acc),
      baseAgl(agl),
      baseRes(res),
      baseCrit(crit),
      baseMaxHp(maxHp) {}

Character::~Character() {}

bool Character::isAlive() const { return hp > 0; }

void Character::takeDamage(int amount) { setHp(hp - amount); }

void Character::heal(int amount) { setHp(hp + amount); }

void Character::takeInfection(int amount) { setInf(inf + amount); }

// Геттеры
int Character::getHp() const { return hp; }
int Character::getMaxHp() const { return maxHp; }
int Character::getAtk() const { return atk; }
int Character::getAcc() const { return acc; }
int Character::getAgl() const { return agl; }
int Character::getRes() const { return res; }
int Character::getInf() const { return inf; }
int Character::getCrit() const { return crit; }

// Сеттеры
void Character::setHp(int newHp) {
  if (newHp <= 0) newHp = 0;
  if (newHp > maxHp) newHp = maxHp;
  hp = newHp;
  // Смерть можно обработать позже
}

void Character::setMaxHp(int newMaxHp) {
  if (newMaxHp <= 0) newMaxHp = 1;
  maxHp = newMaxHp;
  if (hp > maxHp) setHp(maxHp);
}

void Character::setAtk(int newAtk) {
  if (newAtk < 0) newAtk = 0;
  atk = newAtk;
}
void Character::setAcc(int newAcc) {
  if (newAcc < 0) newAcc = 0;
  if (newAcc > 100) newAcc = 100;
  acc = newAcc;
}
void Character::setAgl(int newAgl) {
  if (newAgl < 0) newAgl = 0;
  if (newAgl > 100) newAgl = 100;
  agl = newAgl;
}
void Character::setRes(int newRes) {
  if (newRes < 0) newRes = 0;
  if (newRes > 100) newRes = 100;
  res = newRes;
}
void Character::setCrit(int newCrit) {
  if (newCrit < 0) newCrit = 0;
  if (newCrit > 100) newCrit = 100;
  crit = newCrit;
}

void Character::setInf(int newInf) {
  if (newInf < 0) newInf = 0;
  if (newInf > 100) newInf = 100;
  inf = newInf;
  updateInfectionEffects();
  if (inf == 100) {
    setHp(0);
  }
}

void Character::updateInfectionEffects() {
  // Восстанавливаем базовые значения
  atk = baseAtk;
  acc = baseAcc;
  agl = baseAgl;
  res = baseRes;
  crit = baseCrit;
  maxHp = baseMaxHp;

  // Эффект при INF >= 20: снижение максимального HP на 20%
  if (inf >= 20) {
    maxHp = (baseMaxHp * 80) / 100;
    if (maxHp < 1) maxHp = 1;
    if (hp > maxHp) hp = maxHp;
  }

  // Эффект при INF >= 50: дебафф характеристик
  if (inf >= 50) {
    int multiplier = 100;  // 100% = нет изменений
    if (inf >= 90) {
      multiplier = 50;
    } else {
      multiplier = 140 - inf;
      if (multiplier < 50) multiplier = 50;
      if (multiplier > 100) multiplier = 100;
    }
    atk = (baseAtk * multiplier) / 100;
    acc = (baseAcc * multiplier) / 100;
    agl = (baseAgl * multiplier) / 100;
    crit = (baseCrit * multiplier) / 100;
    if (atk < 0) atk = 0;
    if (acc < 0) acc = 0;
    if (agl < 0) agl = 0;
    if (crit < 0) crit = 0;
  }
  // При INF < 50 характеристики уже восстановлены
}
