#include "SaveLoadSystem.h"

#include <fstream>
#include <string>

#include "GameDatabase_Query.h"
#include "game.h"

bool SaveLoadSystem::SaveGame(Game& game, const std::string& filename) {
  std::ofstream file(filename);

  if (!file) return false;

  file << "ROOM\n";
  file << game.getCurrentRoomId() << "\n";

  file << "HP\n";
  file << game.getHero().getHp() << "\n";

  file << "INF\n";
  file << game.getHero().getInf() << "\n";

  file << "EVENTS\n";

  for (int id : game.getCompletedEvents()) {
    file << id << "\n";
  }

  file << "END_EVENTS\n";

  file << "INVENTORY\n";

  const auto& items = game.getHero().getInventory().getItems();

  for (const auto& item : items) {
    file << item.getId() << "\n";
  }

  file << "END_INVENTORY\n";

  file << "ACTIVE_QUESTS\n";

  for (int id : game.getActiveQuests()) {
    file << id << "\n";
  }

  file << "END_ACTIVE_QUESTS\n";

  file << "COMPLETED_QUESTS\n";

  for (int id : game.getCompletedQuests()) {
    file << id << "\n";
  }

  file << "END_COMPLETED_QUESTS\n";

  file << "NPCS\n";

  for (const auto& npc : game.getDatabase().npcs) {
    file << npc.id << " " << npc.enabled << "\n";
  }

  file << "END_NPCS\n";

  return true;
}

bool SaveLoadSystem::LoadGame(Game& game, const std::string& filename) {
  std::ifstream file(filename);

  if (!file) return false;

  std::string token;

  while (file >> token) {
    if (token == "ROOM") {
      int roomId;
      file >> roomId;

      game.setCurrentRoomId(roomId);
    }

    else if (token == "HP") {
      int hp;
      file >> hp;

      game.getHero().setHp(hp);
    }

    else if (token == "INF") {
      int inf;
      file >> inf;

      game.getHero().setInf(inf);
    }

    else if (token == "EVENTS") {
      game.getCompletedEvents().clear();

      int id;

      while (file >> token) {
        if (token == "END_EVENTS") break;

        id = std::stoi(token);

        game.getCompletedEvents().insert(id);
      }
    }

    else if (token == "INVENTORY") {
      game.getHero().getInventory().clear();

      while (file >> token) {
        if (token == "END_INVENTORY") break;

        int itemId = std::stoi(token);

        const ItemRecord* itemData =
            GameDatabase_Query::FindItemById(game.getDatabase(), itemId);

        if (itemData) {
          Item item = game.getItemFactory().createItem(*itemData);

          game.getHero().getInventory().addItem(item);
        }
      }
    }

    else if (token == "ACTIVE_QUESTS") {
      game.getActiveQuests().clear();

      int id;

      while (file >> token) {
        if (token == "END_ACTIVE_QUESTS") break;

        id = std::stoi(token);

        game.getActiveQuests().insert(id);
      }
    }

    else if (token == "COMPLETED_QUESTS") {
      game.getCompletedQuests().clear();

      int id;

      while (file >> token) {
        if (token == "END_COMPLETED_QUESTS") break;

        id = std::stoi(token);

        game.getCompletedQuests().insert(id);
      }
    }

    else if (token == "NPCS") {
      while (file >> token) {
        if (token == "END_NPCS") break;

        int npcId = std::stoi(token);

        bool enabled;
        file >> enabled;

        NpcRecord* npc =
            GameDatabase_Query::FindNpcById(game.getDatabase(), npcId);

        if (npc) {
          npc->enabled = enabled;
        }
      }
    }
  }

  return true;
}
