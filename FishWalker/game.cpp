#include "game.h"

#include <ctime>
#include <iostream>

#include "ConsoleUI.h"
#include "EventSystem.h"
#include "GameDatabase_Load.h"
#include "GameDatabase_Query.h"
#include "battle.h"
#include "constants.h"

Game::Game()
    : hero("Fisherman", HERO_ID, HERO_START_HP, HERO_MAX_HP, HERO_ATK, HERO_INF,
           nullptr, nullptr),
      running(true),
      currentRoomId(1) {
  srand(static_cast<unsigned>(time(nullptr)));

  GameDatabase_Load::LoadItems(database, "Items.csv");
  GameDatabase_Load::LoadMonsters(database, "Monsters.csv");
  GameDatabase_Load::LoadRooms(database, "Rooms.csv");
  GameDatabase_Load::LoadRoomConnections(database, "RoomConnections.csv");
  GameDatabase_Load::LoadEvents(database, "Events.csv");
  GameDatabase_Load::LoadRoomObjects(database, "RoomObjects.csv");
  GameDatabase_Load::LoadNpcs(database, "Npcs.csv");
  GameDatabase_Load::LoadDialogueNodes(database, "DialogueNodes.csv");
  GameDatabase_Load::LoadDialogueChoices(database, "DialogueChoices.csv");
  GameDatabase_Load::LoadRoomNpcs(database, "RoomNpcs.csv");
}

Game::~Game() {}

void Game::showMainMenu() {
  std::cout << "\n=========================\n";
  std::cout << "        FISHWALKER\n";
  std::cout << "=========================\n";

  const RoomRecord* room =
      GameDatabase_Query::FindRoomById(database, currentRoomId);

  if (room) {
    std::cout << "\nLocation: " << room->title << "\n";
  }

  std::cout << "\nHP: " << hero.getHp() << "/" << hero.getMaxHp();

  std::cout << "\nINF: " << hero.getInf() << "/100\n";

  std::cout << "\n1. Actions";
  std::cout << "\n2. Character";
  std::cout << "\n3. Inventory";
  std::cout << "\n0. Exit";

  std::cout << "\n\n> ";
}

void Game::showInventory() {
  const auto& items = hero.getInventory().getItems();

  if (items.empty()) {
    std::cout << "\nInventory is empty.\n";
    return;
  }

  std::cout << "\n===== INVENTORY =====\n";

  for (size_t i = 0; i < items.size(); i++) {
    std::cout << i + 1 << ". " << items[i].getName() << "\n";
  }

  std::cout << "\n0. Back";
  std::cout << "\n> ";

  int choice = ConsoleUI::ReadInt();

  if (choice > 0 && choice <= static_cast<int>(items.size())) {
    hero.getInventory().useItem(choice - 1, hero);

    std::cout << "\nItem used.\n";
  }
}

void Game::startBattle(Monster* monster) {
  Battle battle(&hero, {monster});

  std::cout << "\nA wild " << monster->getName() << " appears!\n";

  while (!battle.isBattleOver()) {
    std::cout << "\n---------------------\n";

    std::cout << hero.getName() << " HP " << hero.getHp() << "/"
              << hero.getMaxHp() << "\n";

    std::cout << monster->getName() << " HP " << monster->getHp() << "/"
              << monster->getMaxHp() << "\n";

    std::cout << "\n1. Attack";
    std::cout << "\n2. Inventory";
    std::cout << "\n> ";

    int choice = ConsoleUI::ReadInt();

    switch (choice) {
      case 1:
        battle.heroAttack();
        hero.applyDot();
        break;

      case 2:
        showInventory();
        break;
    }
  }

  if (hero.isAlive())
    std::cout << "\nMonster defeated!\n";
  else
    std::cout << "\nYou died!\n";

  delete monster;
}

void Game::triggerEvent(int eventId) {
  EventSystem::ExecuteEvent(*this, eventId);
}

void Game::lookAround() {
  const RoomRecord* room =
      GameDatabase_Query::FindRoomById(database, currentRoomId);

  if (!room) return;

  while (true) {
    ConsoleUI::PrintHeader(room->title);

    std::cout << room->description << "\n";

    auto objects =
        GameDatabase_Query::GetObjectsInRoom(database, currentRoomId);

    if (objects.empty()) {
      std::cout << "\nNothing interesting here.\n";
      return;
    }

    std::cout << "\nObjects:\n";

    for (size_t i = 0; i < objects.size(); i++) {
      std::cout << i + 1 << ". " << objects[i]->name << "\n";
    }

    std::cout << "\n0. Back\n> ";

    int choice = ConsoleUI::ReadInt();

    if (choice == 0) return;

    if (choice < 1 || choice > static_cast<int>(objects.size())) continue;

    const RoomObjectRecord* object = objects[choice - 1];

    ConsoleUI::PrintHeader(object->name);

    std::cout << object->description << "\n";

    if (object->eventId > 0) {
      triggerEvent(object->eventId);
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(10000, '\n');
    std::cin.get();
  }
}

void Game::showCharacter() {
  ConsoleUI::PrintHeader("Character");

  std::cout << "HP: " << hero.getHp() << "/" << hero.getMaxHp() << "\n";

  std::cout << "INF: " << hero.getInf() << "/100\n";
}

void Game::showActionsMenu() {
  while (true) {
    int choice = ConsoleUI::ShowMenu(
        "Actions", {"Look Around", "Move", "Talk", "Interact"});

    switch (choice) {
      case 1:
        lookAround();
        break;

      case 2:
        moveToRoom();
        break;

      case 3:
        showTalkMenu();
        break;

      case 4:
        std::cout << "\nNothing to interact with.\n";
        break;

      case 0:
        return;
    }
  }
}

void Game::moveToRoom() {
  auto connections =
      GameDatabase_Query::GetConnectionsFromRoom(database, currentRoomId);

  if (connections.empty()) {
    std::cout << "\nThere's nowhere to go.\n";
    return;
  }

  ConsoleUI::PrintHeader("Travel");

  for (size_t i = 0; i < connections.size(); i++) {
    std::cout << i + 1 << ". " << connections[i]->choiceText << "\n";
  }

  std::cout << "\n0. Back\n> ";

  int choice = ConsoleUI::ReadInt();

  if (choice <= 0 || choice > static_cast<int>(connections.size())) return;

  currentRoomId = connections[choice - 1]->targetRoomId;

  const RoomRecord* room =
      GameDatabase_Query::FindRoomById(database, currentRoomId);

  if (!room) return;

  std::cout << "\nYou entered: " << room->title << "\n";

  if (room->enterEventId > 0) {
    triggerEvent(room->enterEventId);
  }
}

void Game::showTalkMenu() {
  auto npcs = GameDatabase_Query::GetNpcsInRoom(database, currentRoomId);

  if (npcs.empty()) {
    std::cout << "\nNobody is here.\n";
    return;
  }

  while (true) {
    ConsoleUI::PrintHeader("Talk");

    for (size_t i = 0; i < npcs.size(); i++) {
      std::cout << i + 1 << ". " << npcs[i]->name << "\n";
    }

    std::cout << "\n0. Back\n> ";

    int choice = ConsoleUI::ReadInt();

    if (choice == 0) return;

    if (choice < 1 || choice > static_cast<int>(npcs.size())) continue;

    talkToNpc(npcs[choice - 1]->id);
  }
}

void Game::talkToNpc(int npcId) {
  const NpcRecord* npc = GameDatabase_Query::FindNpcById(database, npcId);

  if (!npc) return;

  while (true) {
    ConsoleUI::PrintHeader(npc->name);

    std::cout << npc->description << "\n";

    std::cout << "\n1. Talk";
    std::cout << "\n0. Back";
    std::cout << "\n> ";

    int choice = ConsoleUI::ReadInt();

    if (choice == 0) return;

    if (choice == 1) {
      startDialogue(npc->firstDialogueNodeId);
      return;
    }
  }
}

void Game::startDialogue(int nodeId) {
  while (nodeId > 0) {
    const DialogueNodeRecord* node =
        GameDatabase_Query::FindDialogueNodeById(database, nodeId);

    if (!node) return;

    const NpcRecord* npc =
        GameDatabase_Query::FindNpcById(database, node->npcId);

    if (npc)
      ConsoleUI::PrintHeader(npc->name);
    else
      ConsoleUI::PrintHeader("Dialogue");

    std::cout << node->text << "\n";

    auto allChoices = GameDatabase_Query::GetChoicesForNode(database, nodeId);

    std::vector<const DialogueChoiceRecord*> choices;

    for (const auto* choice : allChoices) {
      if (choice->requiredEventId > 0 &&
          !isEventCompleted(choice->requiredEventId))
        continue;

      if (choice->forbiddenEventId > 0 &&
          isEventCompleted(choice->forbiddenEventId))
        continue;

      choices.push_back(choice);
    }

    if (choices.empty()) {
      std::cout << "\n(Conversation ended)\n";
      return;
    }

    std::cout << "\n";

    for (size_t i = 0; i < choices.size(); i++) {
      std::cout << i + 1 << ". " << choices[i]->text << "\n";
    }

    std::cout << "\n> ";

    int choice = ConsoleUI::ReadInt();

    if (choice < 1 || choice > static_cast<int>(choices.size())) continue;

    const DialogueChoiceRecord* selected = choices[choice - 1];

    if (selected->eventId > 0) {
      triggerEvent(selected->eventId);
    }

    nodeId = selected->nextNodeId;
  }
}

void Game::run() {
  while (running && hero.isAlive()) {
    showMainMenu();

    int choice = ConsoleUI::ReadInt();

    switch (choice) {
      case 1:
        showActionsMenu();
        break;

      case 2:
        showCharacter();
        break;

      case 3:
        showInventory();
        break;

      case 0:
        running = false;
        break;

      default:
        break;
    }
  }

  std::cout << "\nGame finished.\n";
}

GameDatabase& Game::getDatabase() { return database; }

Hero& Game::getHero() { return hero; }

MonsterFactory& Game::getMonsterFactory() { return monsterFactory; }

ItemFactory& Game::getItemFactory() { return itemFactory; }

bool Game::isEventCompleted(int eventId) const {
  return completedEvents.count(eventId) > 0;
}

void Game::completeEvent(int eventId) { completedEvents.insert(eventId); }
