#ifndef ITEM_H
#define ITEM_H

#include <string>

enum class ItemType { HEAL, WEAPON, ARMOR, ANTIDOTE };

class Item {
 public:
  Item(int id, const std::string& name, ItemType type, int value, int subType,
       int slot, int critChance, int specialType, int specialValue);
  int getId() const;
  const std::string& getName() const;
  ItemType getType() const;
  int getValue() const;
  int getSubType() const;
  int getSlot() const;
  int getCritChance() const;
  int getSpecialType() const;
  int getSpecialValue() const;
  std::string getDescription() const;

 private:
  int id;
  std::string name;
  ItemType type;
  int value;
  int subType;
  int slot;
  int critChance;
  int specialType;
  int specialValue;
};

#endif
