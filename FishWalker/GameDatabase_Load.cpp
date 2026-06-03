#include "GameDatabase_Load.h"

#include "CsvParser.h"
#include "GameDatabase.h"

bool GameDatabase_Load::LoadItems(GameDatabase& db,
                                  const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.items.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); ++i) {
    const auto& row = data[i];

    if (row.size() < 4) continue;

    ItemRecord item;

    item.id = std::stoi(row[0]);
    item.name = row[1];
    item.type = std::stoi(row[2]);
    item.value = std::stoi(row[3]);

    db.items.push_back(item);
  }

  return true;
}

bool GameDatabase_Load::LoadMonsters(GameDatabase& db,
                                     const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.monsters.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); ++i) {
    const auto& row = data[i];

    if (row.size() < 8) continue;

    MonsterRecord monster;

    monster.id = std::stoi(row[0]);
    monster.name = row[1];
    monster.hp = std::stoi(row[2]);
    monster.maxHp = std::stoi(row[3]);
    monster.atk = std::stoi(row[4]);
    monster.inf = std::stoi(row[5]);
    monster.type = std::stoi(row[6]);
    monster.infectionPower = std::stoi(row[7]);

    db.monsters.push_back(monster);
  }

  return true;
}
