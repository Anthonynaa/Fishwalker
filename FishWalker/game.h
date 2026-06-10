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

  std::set<int>& getActiveQuests();

  std::set<int>& getCompletedQuests();

  int getCurrentRoomId() const;

  void setCurrentRoomId(int roomId);

  std::set<int>& getCompletedEvents();

 private:
  Hero hero;

  MonsterFactory monsterFactory;
  ItemFactory itemFactory;

  GameDatabase database;

  bool running;

  int currentRoomId;

  void showMainMenu();
  void showInventory(bool inBattle = false);

  void startBattle(Monster* monster);
  void startBattle(std::vector<Monster*> monsters);

  void triggerEvent(int eventId);

  void showActionsMenu();

  void lookAround();
  void moveToRoom();
  void showCharacter();

  std::set<int> completedEvents;

  void showTalkMenu();
  void startDialogue(int nodeId);
  void talkToNpc(int npcId);

  void showSystemMenu();

  std::set<int> activeQuests;
  std::set<int> completedQuests;
};

#endif
