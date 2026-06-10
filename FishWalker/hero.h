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
  void equipWeapon(Weapon* weapon, int slot);
  void equipArmor(Armor* armor, int slot);
  void clearWeaponSlot(int slot);
  void clearArmorSlot(int slot);
  Weapon* getWeaponInSlot(int slot) const;
  Armor* getArmorInSlot(int slot) const;
  bool isWeaponSlotEmpty(int slot) const;
  bool isArmorSlotEmpty(int slot) const;
  void applyDot();

  void setDamagePercent(int percent) { damagePercent = percent; }
  void resetDamagePercent() { damagePercent = 100; }

  void setHp(int hp);
  void setInf(int inf);

  int getArmorPieces() const { return armorPieces; }
  void addArmorPiece();

  int getHeavyCooldown() const { return heavyCooldown; }
  void setHeavyCooldown(int cd) { heavyCooldown = cd; }
  void decrementCooldown() {
    if (heavyCooldown > 0) heavyCooldown--;
  }

  bool hasWeaponSubType(int subType) const;

 private:
  Weapon* weaponSlots[3];
  Armor* armorSlots[3];
  Inventory inventory;
  int damagePercent = 100;
  int armorPieces = 0;
  int heavyCooldown = 0;
};

#endif
