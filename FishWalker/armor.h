#ifndef ARMOR_H
#define ARMOR_H

#include <string>

class Armor {
 public:
  Armor(const std::string& armorName, int armorDef, int armorType);

  const std::string& getArmorName() const;
  int getArmorDef() const;
  int getArmorType() const;

  void setArmorName(const std::string& newArmorName);
  void setArmorDef(int newArmorDef);
  void setArmorType(int newArmorType);

  virtual ~Armor();

 protected:
  std::string armorName;
  int armorDef, armorType;
};

#endif
