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

}  // namespace GameDatabase_Query

#endif
