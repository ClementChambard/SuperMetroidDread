// Copyright 2023 Clément Chambard
#ifndef ROOM_INCLUDED_H
#define ROOM_INCLUDED_H

#include "BreakableBlock.h"
#include "Collider.h"
#include "CollisionMap.h"
#include "Door.h"
#include "Tilemap.h"
#include <glm/glm.hpp>
#include <json/json.h>
#include <string>
#include <vector>

struct RoomTransition;

class Room {
public:
  void Update();
  void Draw();

  void Init();
  void LoadFromJson(std::string fileName);
  void LoadMetaFromJson(Json::Value const &);
  void checkRoomTransition(Collider *col) const;
  glm::vec2 getRoomPos(glm::vec2 worldpos) const;
  CollisionMap *getCol() { return &collision; }

  CollisionMap collision;
  glm::vec2 worldPos;
  glm::vec2 size;
  std::vector<RoomTransition> transitions;
  std::vector<Door> doors;
  std::vector<BreakableBlock> blocs;
};

struct RoomTransition {
  glm::vec2 pos1, pos2;
  unsigned int roomnumber;
  uint8_t transitionFlags;
  // 0 = right 1 = left 2 = top 3 = bottom
};

#endif
