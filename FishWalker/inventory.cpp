#include "inventory.h"

#include "armor.h"
#include "hero.h"
#include "weapon.h"

Inventory::Inventory() {}

void Inventory::addItem(const Item& item) { items.push_back(item); }

void Inventory::removeItem(int index) {
  if (index >= 0 && index < (int)items.size())
    items.erase(items.begin() + index);
}

bool Inventory::useItem(int index, Hero& hero) {
  if (index < 0 || index >= (int)items.size()) return false;
  Item& item = items[index];
  switch (item.getType()) {
    case ItemType::HEAL:
      hero.heal(item.getValue());
      removeItem(index);
      return true;
    case ItemType::ANTIDOTE:
      hero.takeInfection(-item.getValue());
      removeItem(index);
      return true;
    case ItemType::WEAPON: {
      Weapon* newWeapon = new Weapon(item.getName(), item.getValue(), 0);
      hero.equipWeapon(newWeapon);
      removeItem(index);
      return true;
    }
    case ItemType::ARMOR: {
      Armor* newArmor = new Armor(item.getName(), item.getValue(), 0);
      hero.equipArmor(newArmor);
      removeItem(index);
      return true;
    }
    default:
      return false;
  }
}

void Inventory::clear() { items.clear(); }
