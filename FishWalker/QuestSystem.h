#ifndef QUESTSYSTEM_H
#define QUESTSYSTEM_H

#include <set>

class Game;

class QuestSystem {
 public:
  static void AddQuest(Game& game, int questId);

  static void CompleteQuest(Game& game, int questId);

  static bool IsQuestActive(Game& game, int questId);

  static bool IsQuestCompleted(Game& game, int questId);

  static void ShowQuestLog(Game& game);
};

#endif
