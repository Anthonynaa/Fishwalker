#include "itemFactory.h"

#include "constants.h"

Item ItemFactory::createHealthPotion() const {
  return Item("Health Potion", ItemType::HEAL, HEAL_POTION_AMOUNT);
}
Item ItemFactory::createAntidote() const {
  return Item("Antidote", ItemType::ANTIDOTE, 30);  // уменьшает INF на 30
}
Item ItemFactory::createArmor() const {
  return Item("Leather Armor", ItemType::ARMOR, ARMOR_DEF);
}
Item ItemFactory::createWeapon() const {
  return Item("Knife", ItemType::WEAPON, WEAPON_DAMAGE);
}
Item ItemFactory::createItem() const {
  int r = rand() % 4;
  switch (r) {
    case 0:
      return createHealthPotion();
    case 1:
      return createAntidote();
    case 2:
      return createArmor();
    case 3:
      return createWeapon();
    default:
      return createHealthPotion();
  }
}
