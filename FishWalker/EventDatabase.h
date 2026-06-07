#ifndef EVENTDATABASE_H
#define EVENTDATABASE_H

#include <string>

enum class EventType {
  GIVE_ITEM,
  SPAWN_MONSTER,
  START_QUEST,
  COMPLETE_QUEST,
  UNLOCK_NPC,
  OPEN_SHOP,
  NONE
};

struct EventRecord {
  int id;

  std::string text;

  EventType type;

  int value;

  bool once;
};

#endif
