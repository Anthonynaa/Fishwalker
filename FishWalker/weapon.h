#ifndef WEAPON_H
#define WEAPON_H

#include <string>

class Weapon {
 public:
  // конструктор
  Weapon(const std::string& weaponName, int weaponAtk, int weaponType);

  // геттеры
  const std::string& getWeaponName() const;
  int getWeaponAtk() const;
  int getWeaponType() const;

  // сеттеры
  void setWeaponName(const std::string& newWeaponName);
  void setWeaponAtk(int newWeaponAtk);
  void setWeaponType(int newWeaponType);

  // деструктор
  virtual ~Weapon();

 private:
  std::string weaponName;
  int weaponAtk, weaponType;
};

#endif  // EQUIPMENT_H
