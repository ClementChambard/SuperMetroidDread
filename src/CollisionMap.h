// Copyright 2023 Clément Chambard
#ifndef COLLISIONMAP_INCLUDED_H
#define COLLISIONMAP_INCLUDED_H

#include <glm/glm.hpp>
#include <json/json.h>
#include <string>
#include <vector>

struct CollisionAlteration;

class CollisionMap {
public:
  void fromJsonArray(Json::Value const &val);
  static void FirstInit();

  void Alter(CollisionAlteration *a);
  void UnAlter(CollisionAlteration *a);

  uint8_t getTileAt(float x, float y);
  uint8_t getTileAtNoAlter(float x, float y);
  uint8_t getTileAt(float x, float y, float *xx, float *yy);

  uint8_t getTileAtGlobalPos(float x, float y);
  uint8_t getTileAtGlobalPos(glm::vec2 v) {
    return getTileAtGlobalPos(v.x, v.y);
  }

  bool collideWith(uint8_t t, float xx, float yy);
  bool collideAt(float x, float y);
  bool collideAt(float x, float y, uint8_t *t);

  void draw(glm::vec2 worldpos);

  int xx, yy;

private:
  std::vector<uint8_t> tiles;
  std::vector<CollisionAlteration *> alterations;
  int width, height;
  static constexpr float tilesize = 24;
  static constexpr int tileperblock = 6;
  static bool firstInit;
  static int colsprite0;
};

struct CollisionAlteration {
  std::vector<uint8_t> tiles{};
  int w = 0;
  int h = 0;
  int startx = 0;
  int starty = 0;
};

#endif
