#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include <string>
#include <vector>

struct ItemRecord {
  int id;
  std::string name;
  int type;
  int value;
};

struct MonsterRecord {
  int id;
  std::string name;
  int hp;
  int maxHp;
  int atk;
  int inf;

  int type;
  int infectionPower;
};

class GameDatabase {
 public:
  std::vector<ItemRecord> items;
  std::vector<MonsterRecord> monsters;
};

#endif
