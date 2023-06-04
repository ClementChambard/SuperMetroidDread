// Copyright 2023 Clément Chambard
#include "Collider.h"
#include "Area.h"
#include <math/math.h>

bool Collider::isSlopeBelow() const {
  auto tilebelow =
      Area::GetRoom()->getCol()->getTileAtGlobalPos(pos.x, pos.y - 1);
  return tilebelow != 0 && tilebelow != 16;
}
bool Collider::isHardSlopeBelow() const {
  return hardSlopeLeft || hardSlopeRight;
}

Collider &Collider::Update(glm::vec2 *speed) {
  if (Area::isTransitionning())
    return *this;

  auto colmap = Area::GetRoom()->getCol();

  glm::vec2 actualSpeed = *speed;
  actualSpeed += speedFraction;
  speedFraction =
      actualSpeed - glm::vec2(floor(actualSpeed.x), floor(actualSpeed.y));
  actualSpeed -= speedFraction;

  pos = pos + actualSpeed;
  Area::GetRoom()->checkRoomTransition(this);
  pos = pos - actualSpeed;

  bool onSlope = isSlopeBelow();

  // update in the x direction -> climb slopes
  int dirx = math::sign(actualSpeed.x);
  int i = 0;
  if (dirx > 0) {
    while (actualSpeed.x != i++) {
      pos.x += dirx;
      if (!colmap->getTileAtGlobalPos(topright()) ||
          (!onSlope && !colmap->getTileAtGlobalPos(bottomright())) ||
          !colmap->getTileAtGlobalPos(right())) {
        speed->x = 0;
        speedFraction.x = 0;
        pos.x--;
        break;
      }
    }
  } else {
    while (actualSpeed.x != i--) {
      pos.x += dirx;
      if (!colmap->getTileAtGlobalPos(topleft()) ||
          (!onSlope && !colmap->getTileAtGlobalPos(bottomleft())) ||
          !colmap->getTileAtGlobalPos(left())) {
        speed->x = 0;
        speedFraction.x = 0;
        pos.x++;
        break;
      }
    }
  }
  int diry = math::sign(actualSpeed.y);

  // update in the y direction
  pos.y += actualSpeed.y;
  while (collideAt(top()) || collideAt(topleft()) || collideAt(topright())) {
    pos.y--;
    if (actualSpeed.y > 0) {
      speed->y = 0;
      speedFraction.y = 0;
    }
  }
  grounded = false;
  bool slopeMode = onSlope || (colmap->getTileAtGlobalPos(bottomleft()) % 16) ||
                   (colmap->getTileAtGlobalPos(bottomright()) % 16);
  while (collideAt(pos) ||
         !slopeMode && (collideAt(bottomleft()) || collideAt(bottomright()))) {
    pos.y++;
    if (actualSpeed.y < 0) {
      speed->y = 0;
      speedFraction.y = 0;
      grounded = true;
    }
  }

  // update variables
  glm::vec2 bot1 = pos - glm::vec2(0, 1);
  glm::vec2 lef1 = left() - glm::vec2(1, 0);
  glm::vec2 rig1 = right() + glm::vec2(1, 0);
  glm::vec2 ble1 = bottomleft() + glm::vec2(-1, 1);
  glm::vec2 bri1 = bottomright() + glm::vec2(1, 1);
  // grounded = collideAt(bot1);
  wallLeft = collideAt(lef1);
  wallRight = collideAt(rig1);
  hardSlopeLeft = grounded && colmap->getTileAtGlobalPos(pos) == 1;
  hardSlopeRight = grounded && colmap->getTileAtGlobalPos(pos) == 2;
  canClimb = actualSpeed.y >= 0 && !onSlope &&
             (!wallLeft && collideAt(ble1) || !wallRight && collideAt(bri1));

  return *this;
}

bool Collider::onTile(int x, int y) const {
  x *= 24;
  y *= 24;
  auto p = Area::GetRoom()->getRoomPos(pos * glm::vec2(1, -1));
  if (p.x + size.x / 2.f < x || p.x - size.x / 2.f > x + 24)
    return false;
  return (p.y - size.y <= y + 24 && p.y >= y);
}

bool Collider::collideAt(const glm::vec2 &pos) {
  auto p = Area::GetRoom()->getRoomPos(pos * glm::vec2(1, -1));
  return Area::GetRoom()->getCol()->collideAt(p.x, p.y);
}
