#include "armor.h"

Armor::Armor(const std::string& armorName, int armorDef, int armorType,
             int itemId)
    : armorName(armorName),
      armorDef(armorDef),
      armorType(armorType),
      itemId(itemId) {}

const std::string& Armor::getArmorName() const { return armorName; }
int Armor::getArmorDef() const { return armorDef; }
int Armor::getArmorType() const { return armorType; }
int Armor::getItemId() const { return itemId; }

void Armor::setArmorName(const std::string& newArmorName) {
  armorName = newArmorName;
}
void Armor::setArmorDef(int newArmorDef) { armorDef = newArmorDef; }
void Armor::setArmorType(int newArmorType) { armorType = newArmorType; }

Armor::~Armor() {}
