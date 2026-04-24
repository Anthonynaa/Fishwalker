#ifndef WEAPON_H
#define WEAPON_H

#include <string>

class Weapon {
 public:
  Weapon(const std::string& weaponName, int weaponAtk, int weaponType);

  const std::string& getWeaponName() const;
  int getWeaponAtk() const;
  int getWeaponType() const;

  void setWeaponName(const std::string& newWeaponName);
  void setWeaponAtk(int newWeaponAtk);
  void setWeaponType(int newWeaponType);

  virtual ~Weapon();

 private:
  std::string weaponName;
  int weaponAtk, weaponType;
};

#endif
