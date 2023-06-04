// Copyright 2023 Clément Chambard
#ifndef COLLIDER_INCLUDED_H
#define COLLIDER_INCLUDED_H

#include "CollisionMap.h"
#include <DrawFuncs.h>
#include <glm/glm.hpp>

class Collider {
public:
  Collider() : Collider({0, 0}, {1, 1}) {}
  Collider(const glm::vec2 &pos, const glm::vec2 &size)
      : pos(pos), size(size) {}
  ~Collider() {}

  float w() const { return size.x; }
  float h() const { return size.y; }
  float lx() const { return pos.x - size.x / 2.f; }
  float rx() const { return pos.x + size.x / 2.f; }
  float ty() const { return pos.y + size.y; }
  float by() const { return pos.y; }
  glm::vec2 bottom() const { return pos; }
  glm::vec2 top() const { return pos + size * glm::vec2(0.0f, 1.0f); }
  glm::vec2 left() const { return pos + size * glm::vec2(-0.5f, 0.5f); }
  glm::vec2 right() const { return pos + size * glm::vec2(0.5f, 0.5f); }
  glm::vec2 center() const { return pos + size * glm::vec2(0.0f, 0.5f); }
  glm::vec2 topleft() const { return pos + size * glm::vec2(-0.5f, 1.0f); }
  glm::vec2 topright() const { return pos + size * glm::vec2(0.5f, 1.0f); }
  glm::vec2 bottomleft() const { return pos + size * glm::vec2(-0.5f, 0.0f); }
  glm::vec2 bottomright() const { return pos + size * glm::vec2(0.5f, 0.0f); }

  Collider &setSize(const glm::vec2 &size) {
    this->size = size;
    return *this;
  }
  Collider &setPos(const glm::vec2 &pos) {
    this->pos = pos;
    return *this;
  }
  Collider &Update(glm::vec2 *speed);

  Collider &Draw() {
    NSEngine::draw_quad_2d(topleft(), topright(), bottomright(), bottomleft());
    return *this;
  }

  bool isGrounded() const { return grounded; }
  bool isWallLeft() const { return wallLeft; }
  bool isWallRight() const { return wallRight; }
  bool isHardSlopeL() const { return hardSlopeLeft; }
  bool isHardSlopeR() const { return hardSlopeRight; }
  bool isClimbPos() const { return canClimb; }
  bool isSlopeBelow() const;
  bool isHardSlopeBelow() const;

  bool onTile(int x, int y) const;
  bool onTileBL(int x, int y) const;
  bool onTileBR(int x, int y) const;

  static bool collideAt(const glm::vec2 &pos);

  // private:

  bool grounded;
  bool wallLeft;
  bool wallRight;
  bool hardSlopeLeft;
  bool hardSlopeRight;
  bool canClimb;
  glm::vec2 pos;
  glm::vec2 size;
  glm::vec2 speedFraction = {0, 0};
};

#endif
