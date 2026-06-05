#include "GameDatabase_Load.h"

#include "CsvParser.h"
#include "GameDatabase.h"

bool GameDatabase_Load::LoadItems(GameDatabase& db,
                                  const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.items.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); ++i) {
    const auto& row = data[i];

    if (row.size() < 4) continue;

    ItemRecord item;

    item.id = std::stoi(row[0]);
    item.name = row[1];
    item.type = std::stoi(row[2]);
    item.value = std::stoi(row[3]);

    db.items.push_back(item);
  }

  return true;
}

bool GameDatabase_Load::LoadMonsters(GameDatabase& db,
                                     const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.monsters.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); ++i) {
    const auto& row = data[i];

    if (row.size() < 8) continue;

    MonsterRecord monster;

    monster.id = std::stoi(row[0]);
    monster.name = row[1];
    monster.hp = std::stoi(row[2]);
    monster.maxHp = std::stoi(row[3]);
    monster.atk = std::stoi(row[4]);
    monster.inf = std::stoi(row[5]);
    monster.type = std::stoi(row[6]);
    monster.infectionPower = std::stoi(row[7]);

    db.monsters.push_back(monster);
  }

  return true;
}

bool GameDatabase_Load::LoadRooms(GameDatabase& db,
                                  const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.rooms.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); ++i) {
    const auto& row = data[i];

    if (row.size() < 3) continue;

    RoomRecord room;

    room.id = std::stoi(row[0]);
    room.title = row[1];
    room.description = row[2];

    db.rooms.push_back(room);
  }

  return true;
}

bool GameDatabase_Load::LoadRoomConnections(GameDatabase& db,
                                            const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.roomConnections.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); ++i) {
    const auto& row = data[i];

    if (row.size() < 3) continue;

    RoomConnectionRecord connection;

    connection.roomId = std::stoi(row[0]);
    connection.targetRoomId = std::stoi(row[1]);
    connection.choiceText = row[2];

    db.roomConnections.push_back(connection);
  }

  return true;
}

bool GameDatabase_Load::LoadEvents(GameDatabase& db,
                                   const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.events.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); i++) {
    const auto& row = data[i];

    if (row.size() < 5) continue;

    EventRecord event;

    event.id = std::stoi(row[0]);
    event.text = row[1];
    event.rewardItemId = std::stoi(row[2]);
    event.spawnMonsterId = std::stoi(row[3]);
    event.once = std::stoi(row[4]) != 0;

    db.events.push_back(event);
  }

  return true;
}
bool GameDatabase_Load::LoadRoomObjects(GameDatabase& db,
                                        const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.roomObjects.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); i++) {
    const auto& row = data[i];

    if (row.size() < 5) continue;

    RoomObjectRecord object;

    object.id = std::stoi(row[0]);
    object.roomId = std::stoi(row[1]);
    object.name = row[2];
    object.description = row[3];
    object.eventId = std::stoi(row[4]);

    db.roomObjects.push_back(object);
  }

  return true;
}
