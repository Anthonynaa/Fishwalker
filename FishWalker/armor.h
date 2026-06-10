#ifndef ARMOR_H
#define ARMOR_H

#include <string>

class Armor {
 public:
  Armor(const std::string& armorName, int armorDef, int armorType, int itemId);
  const std::string& getArmorName() const;
  int getArmorDef() const;
  int getArmorType() const;
  int getItemId() const;
  void setArmorName(const std::string& newArmorName);
  void setArmorDef(int newArmorDef);
  void setArmorType(int newArmorType);
  virtual ~Armor();

 private:
  std::string armorName;
  int armorDef, armorType, itemId;
};

#endif
