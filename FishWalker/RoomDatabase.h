#ifndef ROOMDATABASE_H
#define ROOMDATABASE_H

#include <string>

struct RoomRecord {
  int id;

  std::string title;
  std::string description;

  int enterEventId;
};

struct RoomConnectionRecord {
  int roomId;
  int targetRoomId;

  std::string choiceText;
};

#endif
