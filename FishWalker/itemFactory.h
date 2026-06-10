#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include "GameDatabase.h"
#include "armor.h"
#include "item.h"
#include "weapon.h"

class ItemFactory {
 public:
  Item createItem(const ItemRecord& data) const;
  Weapon* createWeapon(const ItemRecord& data) const;
  Armor* createArmor(const ItemRecord& data) const;
};

#endif
