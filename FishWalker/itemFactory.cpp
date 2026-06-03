#include "itemFactory.h"

Item ItemFactory::createItem(const ItemRecord& data) const {
  return Item(data.name, static_cast<ItemType>(data.type), data.value);
}
