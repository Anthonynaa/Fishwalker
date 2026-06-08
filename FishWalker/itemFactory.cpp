#include "itemFactory.h"

Item ItemFactory::createItem(const ItemRecord& record) const {
  return Item(record.id, record.name, static_cast<ItemType>(record.type),
              record.value);
}
