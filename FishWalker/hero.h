#ifndef HERO_H
#define HERO_H

#include <string>

#include "armor.h"
#include "character.h"
#include "inventory.h"
#include "weapon.h"

class Hero : public Character {
 public:
  Hero(const std::string& name, int id, int hp, int maxHp, int atk, int acc,
       int agl, int res, int inf, int crit, Weapon* currentWeapon = nullptr,
       Armor* currentArmor = nullptr);

  Weapon* getCurrentWeapon() const;
  Armor* getCurrentArmor() const;
  Inventory& getInventory();

  void takeDamage(int amount) override;
  void attack(Character& target) override;
  void equipWeapon(Weapon* newWeapon);
  void equipArmor(Armor* newArmor);
  void update() override;

  // Механика заражения: DoT
  void applyDot();

  // Временный множитель для мини-игры
  void setTempDamageMultiplier(int multiplier) { tempMultiplier = multiplier; }
  void resetTempDamageMultiplier() { tempMultiplier = 1; }

  ~Hero();

 private:
  Weapon* currentWeapon;
  Inventory inventory;
  Armor* currentArmor;
  int tempMultiplier = 1;
};

#endif
