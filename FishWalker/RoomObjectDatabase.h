#ifndef ROOMOBJECTDATABASE_H
#define ROOMOBJECTDATABASE_H

#include <string>

struct RoomObjectRecord {
  int id;

  int roomId;

  std::string name;
  std::string description;

  int eventId;
};

#endif
