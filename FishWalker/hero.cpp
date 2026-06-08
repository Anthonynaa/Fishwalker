#include "hero.h"

Hero::Hero(const std::string& name, int id, int hp, int maxHp, int atk, int inf,
           Weapon* currentWeapon, Armor* currentArmor)
    : Character(name, id, hp, maxHp, atk, inf),
      currentWeapon(currentWeapon),
      currentArmor(currentArmor),
      tempMultiplier(1) {}

Hero::~Hero() {
  delete currentWeapon;
  delete currentArmor;
}

Weapon* Hero::getCurrentWeapon() const { return currentWeapon; }
Armor* Hero::getCurrentArmor() const { return currentArmor; }
Inventory& Hero::getInventory() { return inventory; }

void Hero::takeDamage(int amount) {
  int reduced = amount - (currentArmor ? currentArmor->getArmorDef() : 0);
  if (reduced < 0) reduced = 0;
  Character::takeDamage(reduced);
}

void Hero::attack(Character& target) {
  int damage =
      (getAtk() + (currentWeapon ? currentWeapon->getWeaponAtk() : 0)) *
      tempMultiplier;
  if (damage < 0) damage = 0;
  target.takeDamage(damage);
  resetTempDamageMultiplier();
}

void Hero::equipWeapon(Weapon* newWeapon) {
  if (currentWeapon) delete currentWeapon;
  currentWeapon = newWeapon;
}

void Hero::equipArmor(Armor* newArmor) {
  if (currentArmor) delete currentArmor;
  currentArmor = newArmor;
}

void Hero::applyDot() {
  if (getInf() >= 80) {
    int damage = (baseMaxHp / 100);
    if (damage < 1) damage = 1;
    takeDamage(damage);
  }
}

void Hero::setHp(int value) { hp = value; }

void Hero::setInf(int value) { inf = value; }
