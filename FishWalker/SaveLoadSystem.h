#ifndef SAVELOADSYSTEM_H
#define SAVELOADSYSTEM_H

#include <string>

class Game;

class SaveLoadSystem {
 public:
  static bool SaveGame(Game& game, const std::string& filename);

  static bool LoadGame(Game& game, const std::string& filename);
};

#endif
