#include "item.h"

Item::Item(int id, const std::string& name, ItemType type, int value,
           int subType, int slot)
    : id(id),
      name(name),
      type(type),
      value(value),
      subType(subType),
      slot(slot) {}

int Item::getId() const { return id; }
const std::string& Item::getName() const { return name; }
ItemType Item::getType() const { return type; }
int Item::getValue() const { return value; }
int Item::getSubType() const { return subType; }
int Item::getSlot() const { return slot; }

std::string Item::getDescription() const {
  switch (type) {
    case ItemType::HEAL:
      return " (heal +" + std::to_string(value) + " HP)";
    case ItemType::ANTIDOTE:
      return " (antidote -" + std::to_string(value) + " INF)";
    case ItemType::WEAPON:
      return " (+" + std::to_string(value) + " ATK)";
    case ItemType::ARMOR:
      return " (+" + std::to_string(value) + " DEF)";
  }
  return "";
}
