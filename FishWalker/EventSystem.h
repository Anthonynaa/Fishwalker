#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

class Game;

class EventSystem {
 public:
  static void ExecuteEvent(Game& game, int eventId);
};

#endif
