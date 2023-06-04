// Copyright 2023 Clément Chambard
#ifndef BULLET_INCLUDED_H
#define BULLET_INCLUDED_H

#include "AnmPlayer.h"
#include <glm/glm.hpp>

#define MAX_BULLETS 100
#define MAX_MISSILE 50

class Bullet {
public:
  Bullet() {}
  ~Bullet() {}

  void Launch(const glm::vec2 &p, float d, float s, char t);

  void Update();

  void Break();
  bool isActive() const { return active; }
  glm::vec2 getPos() const { return pos; }

  bool active = false;
  glm::vec2 pos;
  glm::vec2 spd;
  unsigned char lifetime = 0;
  char type = 0;
  float d = 0;
  int anm = -1;
  int dmg_src = -1;
  uint8_t id;
};

class Missile {
public:
  Missile() {}
  ~Missile() {}

  void Launch(const glm::vec2 &p, float angle, uint8_t flags);

  void Update();

  void Break();
  bool isActive() const { return (flags & 1) != 0; }

  uint8_t flags = 0;
  glm::vec2 pos{};
  glm::vec2 spd{};
  float speed = 1.f;
  float angle = 0.f;
  uint8_t lifetime = 0;
  int anmID = -1;
  int dmg_src = -1;
  uint8_t id;
};

class Bomb {
public:
  Bomb() {}
  ~Bomb() {}

  void Place(const glm::vec2 &p, uint8_t flags);

  void Update();

  bool isActive() const { return (flags & 1) != 0; }

  uint8_t flags = 0;
  glm::vec2 pos{};
  uint8_t lifetime = 0;
  int anmID = -1;
  int dmg_src = -1;
  uint8_t id;
};

#endif
