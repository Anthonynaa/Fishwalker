#ifndef ITEM_H
#define ITEM_H

#include <string>

enum class ItemType { HEAL, WEAPON, ARMOR, ANTIDOTE };

class Item {
 public:
  Item(int id, const std::string& name, ItemType type, int value);

  int getId() const;

  const std::string& getName() const;
  ItemType getType() const;
  int getValue() const;

  std::string getDescription() const;

 private:
  int id;

  std::string name;
  ItemType type;
  int value;
};

#endif
