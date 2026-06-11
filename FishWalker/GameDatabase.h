#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include <string>
#include <vector>

#include "EventDatabase.h"
#include "NpcDatabase.h"
#include "QuestDatabase.h"
#include "RoomDatabase.h"
#include "RoomObjectDatabase.h"

struct ItemRecord {
  int id;
  std::string name;
  int type;
  int value;
  int subType;
  int slot;
  int critChance;
  int specialType;
  int specialValue;
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

struct MonsterGroupRecord {
  int id;
  std::vector<std::pair<int, int>> monsters;
};

class GameDatabase {
 public:
  std::vector<ItemRecord> items;
  std::vector<MonsterRecord> monsters;
  std::vector<RoomRecord> rooms;
  std::vector<RoomConnectionRecord> roomConnections;
  std::vector<RoomObjectRecord> roomObjects;
  std::vector<EventRecord> events;
  std::vector<NpcRecord> npcs;
  std::vector<DialogueNodeRecord> dialogueNodes;
  std::vector<DialogueChoiceRecord> dialogueChoices;
  std::vector<RoomNpcRecord> roomNpcs;
  std::vector<QuestRecord> quests;
  std::vector<MonsterGroupRecord> monsterGroups;
};

#endif
