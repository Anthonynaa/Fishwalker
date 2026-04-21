#include "inventory.h"

#include <iostream>

#include "armor.h"
#include "hero.h"
#include "weapon.h"

Inventory::Inventory() {}

void Inventory::addItem(const Item& item) { items.push_back(item); }

void Inventory::removeItem(int index) {
  if (index >= 0 && index < items.size()) items.erase(items.begin() + index);
}

bool Inventory::useItem(int index, Hero& hero) {
  if (index < 0 || index >= items.size()) return false;
  Item& item = items[index];
  if (item.getType() == ItemType::HEAL) {
    hero.heal(item.getValue());
    removeItem(index);
    return true;
  } else if (item.getType() == ItemType::ANTIDOTE) {
    hero.takeInfection(-item.getValue());
    removeItem(index);
    return true;
  } else if (item.getType() == ItemType::WEAPON) {
    Weapon* newWeapon = new Weapon(item.getName(), item.getValue(), 0);
    hero.equipWeapon(newWeapon);
    removeItem(index);
    return true;
  } else if (item.getType() == ItemType::ARMOR) {
    Armor* newArmor = new Armor(item.getName(), item.getValue(), 0);
    hero.equipArmor(newArmor);
    removeItem(index);
    return true;
  }
  return false;
}

void Inventory::listItems() const {
  if (items.empty()) {
    std::cout << "Inventory is empty.\n";
    return;
  }
  for (size_t i = 0; i < items.size(); ++i) {
    std::cout << i + 1 << ". " << items[i].getName() << " (";
    if (items[i].getType() == ItemType::HEAL)
      std::cout << "heal +" << items[i].getValue();
    else if (items[i].getType() == ItemType::WEAPON)
      std::cout << "weapon, damage " << items[i].getValue();
    else
      std::cout << "armor, defense " << items[i].getValue();
    std::cout << ")\n";
  }
}
