#include "inventory.h"

#include <iostream>

#include "GameDatabase.h"
#include "GameDatabase_Query.h"
#include "armor.h"
#include "hero.h"
#include "itemFactory.h"
#include "weapon.h"

Inventory::Inventory() {}

void Inventory::addItem(const Item& item) { items.push_back(item); }

void Inventory::removeItem(int index) {
  if (index >= 0 && index < (int)items.size())
    items.erase(items.begin() + index);
}

bool Inventory::useItem(int index, Hero& hero, GameDatabase& db,
                        bool inBattle) {
  if (index < 0 || index >= (int)items.size()) return false;
  Item& item = items[index];

  switch (item.getType()) {
    case ItemType::HEAL:
      hero.heal(item.getValue());
      std::cout << "\nUsed " << item.getName() << ". Healed " << item.getValue()
                << " HP.\n";
      removeItem(index);
      return true;
    case ItemType::ANTIDOTE:
      hero.takeInfection(-item.getValue());
      std::cout << "\nUsed " << item.getName() << ". INF reduced by "
                << item.getValue() << ".\n";
      removeItem(index);
      return true;
    case ItemType::WEAPON: {
      int slot = -1;
      if (item.getSubType() == 0)
        slot = 0;
      else if (item.getSubType() == 1)
        slot = 1;
      else if (item.getSubType() == 2)
        slot = 2;
      if (slot == -1) {
        std::cout << "\nCannot equip this weapon.\n";
        return false;
      }
      if (inBattle && !hero.isWeaponSlotEmpty(slot)) {
        std::cout
            << "\nCannot change weapon during battle! Slot is occupied.\n";
        return false;
      }

      Weapon* oldWeapon = hero.getWeaponInSlot(slot);
      if (oldWeapon) {
        const ItemRecord* oldRec =
            GameDatabase_Query::FindItemById(db, oldWeapon->getItemId());
        if (oldRec) {
          Item oldItem = ItemFactory().createItem(*oldRec);
          hero.getInventory().addItem(oldItem);
          std::cout << "\n"
                    << oldWeapon->getWeaponName()
                    << " returned to inventory.\n";
        }
        delete oldWeapon;
        hero.clearWeaponSlot(slot);
      }

      Weapon* newWeapon =
          new Weapon(item.getName(), item.getValue(), (int)item.getType(),
                     item.getSubType(), item.getId(), item.getCritChance(),
                     item.getSpecialType(), item.getSpecialValue());
      hero.equipWeapon(newWeapon, slot);
      std::cout << "\nEquipped " << item.getName() << " in weapon slot " << slot
                << ".\n";
      removeItem(index);
      return true;
    }
    case ItemType::ARMOR: {
      int slot = item.getSlot();
      if (slot < 0 || slot > 2) {
        std::cout << "\nInvalid armor slot.\n";
        return false;
      }
      if (inBattle && !hero.isArmorSlotEmpty(slot)) {
        std::cout << "\nCannot change armor during battle! Slot is occupied.\n";
        return false;
      }

      hero.addArmorPiece();

      Armor* oldArmor = hero.getArmorInSlot(slot);
      if (oldArmor) {
        const ItemRecord* oldRec =
            GameDatabase_Query::FindItemById(db, oldArmor->getItemId());
        if (oldRec) {
          Item oldItem = ItemFactory().createItem(*oldRec);
          hero.getInventory().addItem(oldItem);
          std::cout << "\n"
                    << oldArmor->getArmorName() << " returned to inventory.\n";
        }
        delete oldArmor;
        hero.clearArmorSlot(slot);
      }

      Armor* newArmor =
          new Armor(item.getName(), item.getValue(), 0, item.getId());
      hero.equipArmor(newArmor, slot);
      std::cout << "\nEquipped " << item.getName() << " on armor slot " << slot
                << ".\n";
      removeItem(index);
      return true;
    }
    default:
      return false;
  }
}

void Inventory::clear() { items.clear(); }
