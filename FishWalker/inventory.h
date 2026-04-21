#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <vector>

#include "item.h"

class Hero;

class Inventory {
 public:
  Inventory();

  void addItem(const Item& item);
  void removeItem(int index);
  bool useItem(int index, Hero& hero);
  void listItems() const;

  const std::vector<Item>& getItems() const { return items; }

 private:
  std::vector<Item> items;
};

#endif  // INVENTORY_H
