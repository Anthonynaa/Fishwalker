#ifndef EVENTDATABASE_H
#define EVENTDATABASE_H

#include <string>

struct EventRecord {
  int id;

  std::string text;

  int rewardItemId;

  int spawnMonsterId;

  bool once;
};

#endif
