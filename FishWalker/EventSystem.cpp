#include "EventSystem.h"

#include <iostream>

#include "GameDatabase_Query.h"
#include "game.h"

void EventSystem::ExecuteEvent(Game& game, int eventId) {
  game.triggerEvent(eventId);
}
