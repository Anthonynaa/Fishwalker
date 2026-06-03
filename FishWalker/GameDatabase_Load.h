#ifndef GAMEDATABASE_LOAD_H
#define GAMEDATABASE_LOAD_H

#include <string>

class GameDatabase;

namespace GameDatabase_Load {

bool LoadItems(GameDatabase& db, const std::string& filename);

bool LoadMonsters(GameDatabase& db, const std::string& filename);

}  // namespace GameDatabase_Load

#endif
