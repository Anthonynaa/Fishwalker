#ifndef NPCDATABASE_H
#define NPCDATABASE_H

#include <string>

struct NpcRecord {
  int id;

  std::string name;
  std::string description;

  int roomId;

  int firstDialogueNodeId;

  bool enabled;
};

struct DialogueNodeRecord {
  int id;

  int npcId;

  std::string text;
};

struct DialogueChoiceRecord {
  int id;

  int nodeId;

  std::string text;

  int nextNodeId;

  int eventId;

  int requiredEventId;

  int forbiddenEventId;
};

struct RoomNpcRecord {
  int roomId;

  int npcId;
};

#endif
