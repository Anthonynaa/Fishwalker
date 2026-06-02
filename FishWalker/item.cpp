#include "item.h"

Item::Item(const std::string& name, ItemType type, int value)
    : name(name), type(type), value(value) {}

const std::string& Item::getName() const { return name; }
ItemType Item::getType() const { return type; }
int Item::getValue() const { return value; }

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
