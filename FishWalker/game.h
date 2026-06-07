#pragma once
#ifndef GAME_H
#define GAME_H

#include <set>

#include "ConsoleUI.h"
#include "GameDatabase.h"
#include "hero.h"
#include "itemFactory.h"
#include "monsterFactory.h"

class Game {
  friend class EventSystem;

 public:
  Game();
  ~Game();

  void run();

  GameDatabase& getDatabase();

  Hero& getHero();

  MonsterFactory& getMonsterFactory();

  ItemFactory& getItemFactory();

  bool isEventCompleted(int eventId) const;

  void completeEvent(int eventId);

 private:
  Hero hero;

  MonsterFactory monsterFactory;
  ItemFactory itemFactory;

  GameDatabase database;

  bool running;

  int currentRoomId;

  void showMainMenu();
  void showInventory();

  void startBattle(Monster* monster);

  void triggerEvent(int eventId);

  void showActionsMenu();

  void lookAround();
  void moveToRoom();
  void showCharacter();

  std::set<int> completedEvents;

  void showTalkMenu();
  void startDialogue(int nodeId);
  void talkToNpc(int npcId);
};

#endif
