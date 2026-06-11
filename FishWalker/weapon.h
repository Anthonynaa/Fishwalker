#ifndef WEAPON_H
#define WEAPON_H

#include <string>

class Weapon {
 public:
  Weapon(const std::string& weaponName, int weaponAtk, int weaponType,
         int weaponSubType, int itemId, int critChance, int specialType,
         int specialValue);
  const std::string& getWeaponName() const;
  int getWeaponAtk() const;
  int getWeaponType() const;
  int getWeaponSubType() const;
  int getItemId() const;
  int getCritChance() const;
  int getSpecialType() const;
  int getSpecialValue() const;
  void setWeaponName(const std::string& newWeaponName);
  void setWeaponAtk(int newWeaponAtk);
  void setWeaponType(int newWeaponType);
  void setWeaponSubType(int newWeaponSubType);
  virtual ~Weapon();

 private:
  std::string weaponName;
  int weaponAtk, weaponType, weaponSubType, itemId;
  int critChance;
  int specialType;
  int specialValue;
};

#endif
