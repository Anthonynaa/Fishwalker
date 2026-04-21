#ifndef ARMOR_H
#define ARMOR_H

#include <string>

class Armor {
 public:
  // конструктор
  Armor(const std::string& armorName, int armorDef, int armorType);

  // геттеры
  const std::string& getArmorName() const;
  int getArmorDef() const;
  int getArmorType() const;

  // сеттеры
  void setArmorName(const std::string& newArmorName);
  void setArmorDef(int newArmorDef);
  void setArmorType(int newArmorType);

  // деструктор
  virtual ~Armor();

 protected:
  std::string armorName;
  int armorDef, armorType;
};

#endif  // ARMOR_H
