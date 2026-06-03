#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include "GameDatabase.h"
#include "item.h"

class ItemFactory {
 public:
  Item createItem(const ItemRecord& data) const;
};

#endif
