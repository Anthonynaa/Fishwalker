#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include "item.h"

class ItemFactory {
 public:
  Item createHealthPotion() const;
  Item createAntidote() const;
  Item createArmor() const;
  Item createWeapon() const;
  Item createItem() const;
};

#endif
