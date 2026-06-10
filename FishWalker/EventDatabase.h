#ifndef EVENTDATABASE_H
#define EVENTDATABASE_H

#include <string>

enum class EventType {
  GIVE_ITEM = 0,
  SPAWN_MONSTER = 1,
  ADD_QUEST = 2,
  COMPLETE_QUEST = 3,
  UNLOCK_NPC = 4,
  OPEN_SHOP = 5,
  SPAWN_MONSTER_DOUBLE = 6,
  NONE = 7,
  SPAWN_MONSTER_GROUP = 8
};

struct EventRecord {
  int id;
  std::string text;
  EventType type;
  int value;
  bool once;
};

#endif
