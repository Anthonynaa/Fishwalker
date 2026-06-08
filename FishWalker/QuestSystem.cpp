#include "QuestSystem.h"

#include <iostream>

#include "ConsoleUI.h"
#include "GameDatabase_Query.h"
#include "game.h"

void QuestSystem::AddQuest(Game& game, int questId) {
  auto& active = game.getActiveQuests();
  auto& completed = game.getCompletedQuests();

  if (active.count(questId)) return;

  if (completed.count(questId)) return;

  active.insert(questId);

  const QuestRecord* quest =
      GameDatabase_Query::FindQuestById(game.getDatabase(), questId);

  if (quest) {
    std::cout << "\nQuest received: " << quest->title << "\n";
  }
}

void QuestSystem::CompleteQuest(Game& game, int questId) {
  auto& active = game.getActiveQuests();
  auto& completed = game.getCompletedQuests();

  active.erase(questId);

  completed.insert(questId);

  const QuestRecord* quest =
      GameDatabase_Query::FindQuestById(game.getDatabase(), questId);

  if (quest) {
    std::cout << "\nQuest completed: " << quest->title << "\n";
  }
}

bool QuestSystem::IsQuestActive(Game& game, int questId) {
  return game.getActiveQuests().count(questId);
}

bool QuestSystem::IsQuestCompleted(Game& game, int questId) {
  return game.getCompletedQuests().count(questId);
}

void QuestSystem::ShowQuestLog(Game& game) {
  ConsoleUI::PrintHeader("Quest Log");

  bool hasActive = false;

  std::cout << "\n=== Active Quests ===\n";

  for (int questId : game.getActiveQuests()) {
    const QuestRecord* quest =
        GameDatabase_Query::FindQuestById(game.getDatabase(), questId);

    if (!quest) continue;

    hasActive = true;

    std::cout << "\n" << quest->title << "\n";

    std::cout << quest->description << "\n";
  }

  if (!hasActive) {
    std::cout << "\nNo active quests.\n";
  }

  bool hasCompleted = false;

  std::cout << "\n=== Completed Quests ===\n";

  for (int questId : game.getCompletedQuests()) {
    const QuestRecord* quest =
        GameDatabase_Query::FindQuestById(game.getDatabase(), questId);

    if (!quest) continue;

    hasCompleted = true;

    std::cout << "\n" << quest->title << "\n";
  }

  if (!hasCompleted) {
    std::cout << "\nNo completed quests.\n";
  }

  std::cout << "\nPress Enter to continue...";

  std::cin.ignore(10000, '\n');
  std::cin.get();
}
