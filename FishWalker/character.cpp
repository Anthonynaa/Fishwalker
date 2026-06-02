#include "character.h"

Character::Character(const std::string& name, int id, int hp, int maxHp,
                     int atk, int inf)
    : name(name),
      id(id),
      hp(hp),
      maxHp(maxHp),
      atk(atk),
      inf(inf),
      baseAtk(atk),
      baseMaxHp(maxHp) {}

Character::~Character() {}

bool Character::isAlive() const { return hp > 0; }

void Character::takeDamage(int amount) { setHp(hp - amount); }
void Character::heal(int amount) { setHp(hp + amount); }
void Character::takeInfection(int amount) { setInf(inf + amount); }

int Character::getHp() const { return hp; }
int Character::getMaxHp() const { return maxHp; }
int Character::getAtk() const { return atk; }
int Character::getInf() const { return inf; }
const std::string& Character::getName() const { return name; }
int Character::getId() const { return id; }

void Character::setHp(int newHp) {
  if (newHp <= 0) newHp = 0;
  if (newHp > maxHp) newHp = maxHp;
  hp = newHp;
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

void Character::setInf(int newInf) {
  if (newInf < 0) newInf = 0;
  if (newInf > 100) newInf = 100;
  inf = newInf;
  updateInfectionEffects();
  if (inf == 100) setHp(0);
}

void Character::updateInfectionEffects() {
  atk = baseAtk;
  maxHp = baseMaxHp;

  if (inf >= 20) {
    maxHp = (baseMaxHp * 80) / 100;
    if (maxHp < 1) maxHp = 1;
    if (hp > maxHp) hp = maxHp;
  }
  if (inf >= 50) {
    int multiplier = (inf >= 90) ? 50 : 140 - inf;
    if (multiplier < 50) multiplier = 50;
    if (multiplier > 100) multiplier = 100;
    atk = (baseAtk * multiplier) / 100;
    if (atk < 0) atk = 0;
  }
}
