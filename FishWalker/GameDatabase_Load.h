#ifndef GAMEDATABASE_LOAD_H
#define GAMEDATABASE_LOAD_H

#include <string>

class GameDatabase;

namespace GameDatabase_Load {

bool LoadItems(GameDatabase& db, const std::string& filename);

bool LoadMonsters(GameDatabase& db, const std::string& filename);

bool LoadRooms(GameDatabase& db, const std::string& filename);

bool LoadRoomConnections(GameDatabase& db, const std::string& filename);

bool LoadEvents(GameDatabase& db, const std::string& filename);

bool LoadRoomObjects(GameDatabase& db, const std::string& filename);

bool LoadNpcs(GameDatabase& db, const std::string& filename);

bool LoadDialogueNodes(GameDatabase& db, const std::string& filename);

bool LoadDialogueChoices(GameDatabase& db, const std::string& filename);

bool LoadRoomNpcs(GameDatabase& db, const std::string& filename);

bool LoadQuests(GameDatabase& db, const std::string& filename);

bool LoadMonsterGroups(GameDatabase& db, const std::string& filename);

}  // namespace GameDatabase_Load

#endif
