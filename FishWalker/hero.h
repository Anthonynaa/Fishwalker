#ifndef HERO_H
#define HERO_H

#include <string>

#include "armor.h"
#include "character.h"
#include "inventory.h"
#include "weapon.h"

class Hero : public Character {
 public:
  Hero(const std::string& name, int id, int hp, int maxHp, int atk, int inf,
       Weapon* currentWeapon = nullptr, Armor* currentArmor = nullptr);
  ~Hero();

  Weapon* getCurrentWeapon() const;
  Armor* getCurrentArmor() const;
  Inventory& getInventory();

  void takeDamage(int amount) override;
  void attack(Character& target) override;
  void equipWeapon(Weapon* newWeapon);
  void equipArmor(Armor* newArmor);
  void applyDot();

  void setTempDamageMultiplier(int multiplier) { tempMultiplier = multiplier; }
  void resetTempDamageMultiplier() { tempMultiplier = 1; }

  void setHp(int hp);
  void setInf(int inf);

 private:
  Weapon* currentWeapon;
  Inventory inventory;
  Armor* currentArmor;
  int tempMultiplier = 1;
};

#endif
