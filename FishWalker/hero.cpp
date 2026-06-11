#include "hero.h"

#include <iostream>

Hero::Hero(const std::string& name, int id, int hp, int maxHp, int atk, int inf,
           Weapon* currentWeapon, Armor* currentArmor)
    : Character(name, id, hp, maxHp, atk, inf) {
  for (int i = 0; i < 3; ++i) {
    weaponSlots[i] = nullptr;
    armorSlots[i] = nullptr;
  }
}

Hero::~Hero() {
  for (int i = 0; i < 3; ++i) {
    delete weaponSlots[i];
    delete armorSlots[i];
  }
}

Weapon* Hero::getCurrentWeapon() const { return weaponSlots[0]; }
Armor* Hero::getCurrentArmor() const { return armorSlots[0]; }
Inventory& Hero::getInventory() { return inventory; }

void Hero::takeDamage(int amount) {
  int reduced = amount;
  for (int i = 0; i < 3; ++i) {
    if (armorSlots[i]) reduced -= armorSlots[i]->getArmorDef();
  }
  if (reduced < 0) reduced = 0;
  Character::takeDamage(reduced);
}

void Hero::attack(Character& target) {
  int base = getAtk();
  Weapon* w = weaponSlots[0];
  if (w) base += w->getWeaponAtk();
  int damage = (base * damagePercent) / 100;
  if (damage < 0) damage = 0;
  target.takeDamage(damage);
  resetDamagePercent();
}

void Hero::equipWeapon(Weapon* weapon, int slot) {
  if (slot >= 0 && slot < 3) {
    weaponSlots[slot] = weapon;
  }
}

void Hero::equipArmor(Armor* armor, int slot) {
  if (slot >= 0 && slot < 3) {
    armorSlots[slot] = armor;
  }
}

void Hero::clearWeaponSlot(int slot) {
  if (slot >= 0 && slot < 3) weaponSlots[slot] = nullptr;
}

void Hero::clearArmorSlot(int slot) {
  if (slot >= 0 && slot < 3) armorSlots[slot] = nullptr;
}

Weapon* Hero::getWeaponInSlot(int slot) const {
  return (slot >= 0 && slot < 3) ? weaponSlots[slot] : nullptr;
}

Armor* Hero::getArmorInSlot(int slot) const {
  return (slot >= 0 && slot < 3) ? armorSlots[slot] : nullptr;
}

bool Hero::isWeaponSlotEmpty(int slot) const {
  return (slot >= 0 && slot < 3) && weaponSlots[slot] == nullptr;
}

bool Hero::isArmorSlotEmpty(int slot) const {
  return (slot >= 0 && slot < 3) && armorSlots[slot] == nullptr;
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

void Hero::addArmorPiece() {
  if (armorPieces < 3) {
    ++armorPieces;
    addMaxHp(20);
    std::cout
        << "\nArmor piece collected! Max HP increased by 20. Total pieces: "
        << armorPieces << "\n";
  }
}

bool Hero::hasWeaponSubType(int subType) const {
  for (int i = 0; i < 3; ++i) {
    if (weaponSlots[i] && weaponSlots[i]->getWeaponSubType() == subType)
      return true;
  }
  return false;
}
