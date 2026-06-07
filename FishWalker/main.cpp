#include "game.h"

int main() {
  Game game;
  game.run();

  return 0;
}

// DialogueSystem -> QuestSystem -> SaveLoadSystem

// Осталось имплементировать ивенты в диалоги.

// В Actions есть Interact который ничего не делает. Нужен ли он или нет?
// GameDatabase_Load - много копипасты, нужно будет подстроить под template
