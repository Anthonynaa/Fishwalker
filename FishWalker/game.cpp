#include "game.h"

#include <conio.h>
#include <windows.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "ConsoleUI.h"
#include "EventSystem.h"
#include "GameDatabase_Load.h"
#include "GameDatabase_Query.h"
#include "QuestSystem.h"
#include "SaveLoadSystem.h"
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
  GameDatabase_Load::LoadQuests(database, "Quests.csv");
  GameDatabase_Load::LoadMonsterGroups(database, "MonsterGroups.csv");
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
  std::cout << "\n4. System";

  std::cout << "\n\n> ";
}

void Game::showInventory(bool inBattle) {
  system("cls");
  const auto& items = hero.getInventory().getItems();

  if (items.empty()) {
    std::cout << "\nInventory is empty.\n";
  } else {
    std::cout << "\n===== INVENTORY =====\n";
    for (size_t i = 0; i < items.size(); i++) {
      std::cout << i + 1 << ". " << items[i].getName()
                << items[i].getDescription() << "\n";
    }
    std::cout << "\n0. Back\n";
    std::cout << "> ";

    int choice = ConsoleUI::ReadInt();
    if (choice > 0 && choice <= static_cast<int>(items.size())) {
      hero.getInventory().useItem(choice - 1, hero, database, inBattle);
    }
  }
  ConsoleUI::Pause();
  system("cls");
}

void Game::startBattle(Monster* monster) {
  startBattle(std::vector<Monster*>{monster});
}

void Game::startBattle(std::vector<Monster*> monsters) {
  Battle battle(&hero, monsters);
  if (monsters.size() == 1) {
    std::cout << "\nA wild " << monsters[0]->getName() << " appears!\n";
  } else {
    std::cout << "\n" << monsters.size() << " enemies appear!\n";
  }
  ConsoleUI::Pause();

  while (!battle.isBattleOver()) {
    int actionsLeft = battle.countAliveEnemies();
    while (actionsLeft > 0 && !battle.isBattleOver()) {
      system("cls");
      std::cout << "\n---------------------\n";
      std::cout << hero.getName() << " HP " << hero.getHp() << "/"
                << hero.getMaxHp() << "\n";
      for (auto* m : monsters) {
        if (m->isAlive()) {
          std::cout << m->getName() << " HP " << m->getHp() << "/"
                    << m->getMaxHp() << "\n";
        }
      }
      std::cout << "\nActions remaining: " << actionsLeft;
      std::cout << "\n1. Rod attack\n2. Net attack (splash)\n3. Heavy weapon";
      if (hero.getHeavyCooldown() > 0) {
        std::cout << " (cooldown: " << hero.getHeavyCooldown() << ")";
      }
      std::cout << "\n4. Inventory\n> ";

      int choice = ConsoleUI::ReadInt();
      switch (choice) {
        case 1:
          battle.rodAttack();
          hero.applyDot();
          actionsLeft--;
          break;
        case 2:
          if (!hero.hasWeaponSubType(1)) {
            std::cout << "\nYou need a net equipped to use Net attack!\n";
            ConsoleUI::Pause();
          } else {
            battle.netAttack();
            hero.applyDot();
            actionsLeft--;
          }
          break;
        case 3:
          if (!hero.hasWeaponSubType(2)) {
            std::cout << "\nYou need a heavy weapon (harpoon) equipped to use "
                         "Heavy attack!\n";
            ConsoleUI::Pause();
          } else {
            battle.heavyAttack();
            hero.applyDot();
            if (hero.getHeavyCooldown() == 0) actionsLeft--;
          }
          break;
        case 4:
          showInventory(true);
          break;
      }
      if (!hero.isAlive()) break;
    }
    if (battle.isBattleOver()) break;
    battle.monstersAttack();
    hero.decrementCooldown();
    hero.applyDot();
    ConsoleUI::Pause();
  }

  if (hero.isAlive())
    std::cout << "\nVictory!\n";
  else
    std::cout << "\nYou died!\n";

  ConsoleUI::Pause();

  for (auto* m : monsters) delete m;
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

      ConsoleUI::Pause();

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
    ConsoleUI::Pause();
  }
}

void Game::showCharacter() {
  ConsoleUI::PrintHeader("Character");
  std::cout << "HP: " << hero.getHp() << "/" << hero.getMaxHp() << "\n";
  std::cout << "ATK: " << hero.getAtk() << "\n";
  std::cout << "INF: " << hero.getInf() << "/100\n";
  std::cout << "Armor Pieces collected: " << hero.getArmorPieces() << "/3\n";

  std::cout << "\n--- Armor Slots ---\n";
  const char* armorSlotNames[] = {"Head", "Chest", "Legs"};
  for (int i = 0; i < 3; ++i) {
    Armor* a = hero.getArmorInSlot(i);
    std::cout << armorSlotNames[i] << ": ";
    if (a)
      std::cout << a->getArmorName() << " (DEF +" << a->getArmorDef() << ")";
    else
      std::cout << "empty";
    std::cout << "\n";
  }

  std::cout << "\n--- Weapon Slots ---\n";
  const char* weaponSlotNames[] = {"Rod", "Net", "Heavy"};
  for (int i = 0; i < 3; ++i) {
    Weapon* w = hero.getWeaponInSlot(i);
    std::cout << weaponSlotNames[i] << ": ";
    if (w)
      std::cout << w->getWeaponName() << " (ATK +" << w->getWeaponAtk() << ")";
    else
      std::cout << "empty";
    std::cout << "\n";
  }

  ConsoleUI::Pause();
}

void Game::showActionsMenu() {
  while (true) {
    int choice = ConsoleUI::ShowMenu("Actions",
                                     {"Look Around", "Move", "Talk", "Quests"});
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
        QuestSystem::ShowQuestLog(*this);
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

    ConsoleUI::Pause();

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

    ConsoleUI::Pause();

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

void Game::showSystemMenu() {
  while (true) {
    int choice =
        ConsoleUI::ShowMenu("System", {"Save Game", "Load Game", "Exit Game"});
    switch (choice) {
      case 1:
        if (SaveLoadSystem::SaveGame(*this, "save.txt"))
          std::cout << "\nGame saved.\n";
        else
          std::cout << "\nFailed to save.\n";
        break;
      case 2:
        if (SaveLoadSystem::LoadGame(*this, "save.txt"))
          std::cout << "\nGame loaded.\n";
        else
          std::cout << "\nFailed to load.\n";
        break;
      case 3:
        running = false;
        return;
      case 0:
        return;
    }
  }
}

void Game::talkToNpc(int npcId) {
  const NpcRecord* npc = GameDatabase_Query::FindNpcById(database, npcId);
  if (!npc) return;

  while (true) {
    ConsoleUI::PrintHeader(npc->name);
    std::cout << npc->description << "\n";
    std::cout << "\n1. Talk\n0. Back\n> ";
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
      case 4:
        showSystemMenu();
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

std::set<int>& Game::getActiveQuests() { return activeQuests; }
std::set<int>& Game::getCompletedQuests() { return completedQuests; }
int Game::getCurrentRoomId() const { return currentRoomId; }
void Game::setCurrentRoomId(int roomId) { currentRoomId = roomId; }
std::set<int>& Game::getCompletedEvents() { return completedEvents; }
