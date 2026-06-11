#include "GameDatabase_Load.h"

#include <unordered_map>

#include "CsvParser.h"
#include "GameDatabase.h"

bool GameDatabase_Load::LoadItems(GameDatabase& db,
                                  const std::string& filename) {
  auto data = CsvParser::parseFile(filename);
  db.items.clear();
  if (data.size() <= 1) return false;
  for (size_t i = 1; i < data.size(); ++i) {
    const auto& row = data[i];
    if (row.size() < 9) continue;
    ItemRecord item;
    item.id = std::stoi(row[0]);
    item.name = row[1];
    item.type = std::stoi(row[2]);
    item.value = std::stoi(row[3]);
    item.subType = std::stoi(row[4]);
    item.slot = std::stoi(row[5]);
    item.critChance = std::stoi(row[6]);
    item.specialType = std::stoi(row[7]);
    item.specialValue = std::stoi(row[8]);
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
    if (row.size() < 4) continue;
    RoomRecord room;
    room.id = std::stoi(row[0]);
    room.title = row[1];
    room.description = row[2];
    room.enterEventId = std::stoi(row[3]);
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

EventType ParseEventType(const std::string& str) {
  static const std::unordered_map<std::string, EventType> eventTypes = {
      {"GIVE_ITEM", EventType::GIVE_ITEM},
      {"SPAWN_MONSTER", EventType::SPAWN_MONSTER},
      {"ADD_QUEST", EventType::ADD_QUEST},
      {"COMPLETE_QUEST", EventType::COMPLETE_QUEST},
      {"UNLOCK_NPC", EventType::UNLOCK_NPC},
      {"OPEN_SHOP", EventType::OPEN_SHOP},
      {"SPAWN_MONSTER_GROUP", EventType::SPAWN_MONSTER_GROUP}};

  auto it = eventTypes.find(str);

  if (it != eventTypes.end()) return it->second;

  return EventType::NONE;
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
    event.type = ParseEventType(row[2]);
    event.value = std::stoi(row[3]);
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

bool GameDatabase_Load::LoadNpcs(GameDatabase& db,
                                 const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.npcs.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); i++) {
    const auto& row = data[i];

    if (row.size() < 5) continue;

    NpcRecord npc;

    npc.id = std::stoi(row[0]);
    npc.name = row[1];
    npc.description = row[2];
    npc.roomId = std::stoi(row[3]);
    npc.firstDialogueNodeId = std::stoi(row[4]);
    npc.enabled = std::stoi(row[5]) != 0;

    db.npcs.push_back(npc);
  }

  return true;
}

bool GameDatabase_Load::LoadDialogueNodes(GameDatabase& db,
                                          const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.dialogueNodes.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); i++) {
    const auto& row = data[i];

    if (row.size() < 3) continue;

    DialogueNodeRecord node;

    node.id = std::stoi(row[0]);
    node.npcId = std::stoi(row[1]);
    node.text = row[2];

    db.dialogueNodes.push_back(node);
  }

  return true;
}

bool GameDatabase_Load::LoadDialogueChoices(GameDatabase& db,
                                            const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.dialogueChoices.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); i++) {
    const auto& row = data[i];

    if (row.size() < 7) continue;

    DialogueChoiceRecord choice;

    choice.id = std::stoi(row[0]);
    choice.nodeId = std::stoi(row[1]);
    choice.text = row[2];
    choice.nextNodeId = std::stoi(row[3]);
    choice.eventId = std::stoi(row[4]);
    choice.requiredEventId = std::stoi(row[5]);
    choice.forbiddenEventId = std::stoi(row[6]);

    db.dialogueChoices.push_back(choice);
  }

  return true;
}

bool GameDatabase_Load::LoadRoomNpcs(GameDatabase& db,
                                     const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.roomNpcs.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); i++) {
    const auto& row = data[i];

    if (row.size() < 2) continue;

    RoomNpcRecord roomNpc;

    roomNpc.roomId = std::stoi(row[0]);
    roomNpc.npcId = std::stoi(row[1]);

    db.roomNpcs.push_back(roomNpc);
  }

  return true;
}

bool GameDatabase_Load::LoadQuests(GameDatabase& db,
                                   const std::string& filename) {
  auto data = CsvParser::parseFile(filename);

  db.quests.clear();

  if (data.size() <= 1) return false;

  for (size_t i = 1; i < data.size(); i++) {
    const auto& row = data[i];

    if (row.size() < 3) continue;

    QuestRecord quest;

    quest.id = std::stoi(row[0]);
    quest.title = row[1];
    quest.description = row[2];

    db.quests.push_back(quest);
  }

  return true;
}

bool GameDatabase_Load::LoadMonsterGroups(GameDatabase& db,
                                          const std::string& filename) {
  auto data = CsvParser::parseFile(filename);
  db.monsterGroups.clear();
  if (data.size() <= 1) return false;
  for (size_t i = 1; i < data.size(); ++i) {
    const auto& row = data[i];
    if (row.size() < 3) continue;
    int groupId = std::stoi(row[0]);
    int monsterId = std::stoi(row[1]);
    int count = std::stoi(row[2]);
    auto it = std::find_if(
        db.monsterGroups.begin(), db.monsterGroups.end(),
        [groupId](const MonsterGroupRecord& g) { return g.id == groupId; });
    if (it == db.monsterGroups.end()) {
      MonsterGroupRecord newGroup;
      newGroup.id = groupId;
      newGroup.monsters.push_back({monsterId, count});
      db.monsterGroups.push_back(newGroup);
    } else {
      it->monsters.push_back({monsterId, count});
    }
  }
  return true;
}
