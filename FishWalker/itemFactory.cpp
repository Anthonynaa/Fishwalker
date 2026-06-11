#include "itemFactory.h"

Item ItemFactory::createItem(const ItemRecord& record) const {
  return Item(record.id, record.name, static_cast<ItemType>(record.type),
              record.value, record.subType, record.slot, record.critChance,
              record.specialType, record.specialValue);
}

Weapon* ItemFactory::createWeapon(const ItemRecord& record) const {
  return new Weapon(record.name, record.value, record.type, record.subType,
                    record.id, record.critChance, record.specialType,
                    record.specialValue);
}

Armor* ItemFactory::createArmor(const ItemRecord& record) const {
  return new Armor(record.name, record.value, record.type, record.id);
}
