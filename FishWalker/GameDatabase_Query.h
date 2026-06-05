#ifndef GAMEDATABASE_QUERY_H
#define GAMEDATABASE_QUERY_H

#include <vector>

#include "GameDatabase.h"

namespace GameDatabase_Query {

template <typename T>
const T* FindById(const std::vector<T>& vec, int id) {
  for (const auto& item : vec)
    if (item.id == id) return &item;

  return nullptr;
}

inline const MonsterRecord* FindMonsterById(const GameDatabase& db, int id) {
  return FindById(db.monsters, id);
}

inline const ItemRecord* FindItemById(const GameDatabase& db, int id) {
  return FindById(db.items, id);
}

inline const RoomRecord* FindRoomById(const GameDatabase& db, int id) {
  return FindById(db.rooms, id);
}

inline std::vector<const RoomConnectionRecord*> GetConnectionsFromRoom(
    const GameDatabase& db, int roomId) {
  std::vector<const RoomConnectionRecord*> result;

  for (const auto& connection : db.roomConnections) {
    if (connection.roomId == roomId) {
      result.push_back(&connection);
    }
  }

  return result;
}

inline const EventRecord* FindEventById(const GameDatabase& db, int id) {
  return FindById(db.events, id);
}

inline std::vector<const RoomObjectRecord*> GetObjectsInRoom(
    const GameDatabase& db, int roomId) {
  std::vector<const RoomObjectRecord*> result;

  for (const auto& object : db.roomObjects) {
    if (object.roomId == roomId) result.push_back(&object);
  }

  return result;
}

}  // namespace GameDatabase_Query

#endif
