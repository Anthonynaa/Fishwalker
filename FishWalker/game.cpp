#include "game.h"

#include <ctime>
#include <iostream>

#include "CsvParser.h"
#include "GameDatabase.h"
#include "GameDatabase_Load.h"
#include "GameDatabase_Query.h"
#include "battle.h"
#include "constants.h"

Game::Game()
    : hero("Fisherman", HERO_ID, HERO_START_HP, HERO_MAX_HP, HERO_ATK, HERO_INF,
           nullptr, nullptr),
      running(true) {
  srand(static_cast<unsigned>(time(nullptr)));

  GameDatabase_Load::LoadItems(database, "Items.csv");
  GameDatabase_Load::LoadMonsters(database, "Monsters.csv");
}

Game::~Game() {}

void Game::showMainMenu() {
  std::cout << "\n=========================\n";
  std::cout << "        FISHWALKER\n";
  std::cout << "=========================\n";

  std::cout << "HP: " << hero.getHp() << "/" << hero.getMaxHp() << "\n";

  std::cout << "INF: " << hero.getInf() << "/100\n";

  std::cout << "\n1. Explore";
  std::cout << "\n2. Inventory";
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

  int choice;
  std::cin >> choice;

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

    int choice;
    std::cin >> choice;

    switch (choice) {
      case 1:
        battle.heroAttack();
        hero.applyDot();
        break;

      case 2:
        showInventory();
        break;

      default:
        break;
    }
  }

  if (hero.isAlive()) {
    std::cout << "\nMonster defeated!\n";
  } else {
    std::cout << "\nYou died!\n";
  }

  delete monster;
}

void Game::explore() {
  int eventRoll = rand() % 100;

  if (eventRoll < 60) {
    if (database.monsters.empty()) {
      std::cout << "No monsters loaded!\n";
      return;
    }

    int index = rand() % database.monsters.size();

    Monster* monster = monsterFactory.createMonster(database.monsters[index]);

    startBattle(monster);
  } else {
    if (database.items.empty()) {
      std::cout << "No items loaded!\n";
      return;
    }

    int index = rand() % database.items.size();

    Item loot = itemFactory.createItem(database.items[index]);

    std::cout << "\nYou found: " << loot.getName() << "\n";

    hero.getInventory().addItem(loot);
  }
}

void Game::run() {
  GameDatabase db;

  GameDatabase_Load::LoadMonsters(db, "Monsters.csv");

  const MonsterRecord* monsterData = GameDatabase_Query::FindMonsterById(db, 2);

  if (monsterData) {
    Monster* monster = monsterFactory.createMonster(*monsterData);

    std::cout << monster->getName() << "\n";

    delete monster;
  }
  while (running && hero.isAlive()) {
    showMainMenu();

    int choice;
    std::cin >> choice;

    switch (choice) {
      case 1:
        explore();
        break;

      case 2:
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
