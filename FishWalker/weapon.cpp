#include "weapon.h"

Weapon::Weapon(const std::string& weaponName, int weaponAtk, int weaponType)
    : weaponName(weaponName), weaponAtk(weaponAtk), weaponType(weaponType) {}

const std::string& Weapon::getWeaponName() const { return weaponName; }
int Weapon::getWeaponAtk() const { return weaponAtk; }
int Weapon::getWeaponType() const { return weaponType; }

void Weapon::setWeaponName(const std::string& newWeaponName) {
  weaponName = newWeaponName;
}
void Weapon::setWeaponAtk(int newWeaponAtk) { weaponAtk = newWeaponAtk; }
void Weapon::setWeaponType(int newWeaponType) { weaponType = newWeaponType; }

Weapon::~Weapon() {}
