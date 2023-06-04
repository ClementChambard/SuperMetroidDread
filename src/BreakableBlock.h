// Copyright 2023 Clément Chambard
#ifndef BREAKABLEBLOCK_H_
#define BREAKABLEBLOCK_H_

#include "CollisionMap.h"

enum class BlockType {
  BEAM = 0,
  CRUMBLE = 1,
  MISSILE = 2,
  BOMB = 3,
  PBOMB = 4,
  SCREW = 5,
  SPEEDBOOST = 6
};

class BreakableBlock {
public:
  BreakableBlock(BlockType t, int gridX, int gridY, CollisionMap *map);
  ~BreakableBlock();

  void Update();
  void Draw();

  BlockType type;
  glm::vec2 pos{};
  int reformTimer = -1;
  int anmid = 0;
  CollisionAlteration col{};
  CollisionMap *map;
};

#endif // BREAKABLEBLOCK_H_
