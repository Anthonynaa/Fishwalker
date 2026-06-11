#include "weapon.h"

Weapon::Weapon(const std::string& weaponName, int weaponAtk, int weaponType,
               int weaponSubType, int itemId, int critChance, int specialType,
               int specialValue)
    : weaponName(weaponName),
      weaponAtk(weaponAtk),
      weaponType(weaponType),
      weaponSubType(weaponSubType),
      itemId(itemId),
      critChance(critChance),
      specialType(specialType),
      specialValue(specialValue) {}

const std::string& Weapon::getWeaponName() const { return weaponName; }
int Weapon::getWeaponAtk() const { return weaponAtk; }
int Weapon::getWeaponType() const { return weaponType; }
int Weapon::getWeaponSubType() const { return weaponSubType; }
int Weapon::getItemId() const { return itemId; }
int Weapon::getCritChance() const { return critChance; }
int Weapon::getSpecialType() const { return specialType; }
int Weapon::getSpecialValue() const { return specialValue; }

void Weapon::setWeaponName(const std::string& newWeaponName) {
  weaponName = newWeaponName;
}
void Weapon::setWeaponAtk(int newWeaponAtk) { weaponAtk = newWeaponAtk; }
void Weapon::setWeaponType(int newWeaponType) { weaponType = newWeaponType; }
void Weapon::setWeaponSubType(int newWeaponSubType) {
  weaponSubType = newWeaponSubType;
}

Weapon::~Weapon() {}
