#include "EventSystem.h"

#include <iostream>

#include "GameDatabase_Query.h"
#include "QuestSystem.h"
#include "game.h"

void EventSystem::ExecuteEvent(Game& game, int eventId) {
  const EventRecord* event =
      GameDatabase_Query::FindEventById(game.getDatabase(), eventId);

  if (!event) return;

  if (event->once && game.isEventCompleted(eventId)) return;

  std::cout << "\n" << event->text << "\n";

  switch (event->type) {
    case EventType::GIVE_ITEM: {
      const ItemRecord* itemData =
          GameDatabase_Query::FindItemById(game.getDatabase(), event->value);

      if (itemData) {
        Item item = game.getItemFactory().createItem(*itemData);

        game.getHero().getInventory().addItem(item);

        std::cout << "\nReceived: " << item.getName() << "\n";
      }

      break;
    }

    case EventType::SPAWN_MONSTER: {
      const MonsterRecord* monsterData =
          GameDatabase_Query::FindMonsterById(game.getDatabase(), event->value);

      if (monsterData) {
        Monster* monster = game.getMonsterFactory().createMonster(*monsterData);

        game.startBattle(monster);
      }

      break;
    }

    case EventType::UNLOCK_NPC: {
      NpcRecord* npc = GameDatabase_Query::FindNpcByIdMutable(
          game.getDatabase(), event->value);

      if (npc) {
        npc->enabled = true;
      }

      break;
    }
    case EventType::ADD_QUEST:
      QuestSystem::AddQuest(game, event->value);
      break;

    case EventType::COMPLETE_QUEST:
      QuestSystem::CompleteQuest(game, event->value);
      break;

    case EventType::OPEN_SHOP:
      break;

    case EventType::NONE:
      break;

    default:
      break;
  }

  game.completeEvent(eventId);
}
