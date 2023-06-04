// Copyright 2023 Clément Chambard
#include "Bullet.h"
#include "Samus.h"

#include "AnmEffects.h"
#include "Collider.h"
#include <DrawFuncs.h>

#define BULLET_MAX_LIFE 200

void Bullet::Launch(const glm::vec2 &p, float d, float s, char t) {
  // if (Collider::collideAt(p)) return;
  lifetime = 0;
  this->d = d;
  active = true;
  pos = p;
  type = t;
  spd = glm::vec2(s * cos(d), s * sin(d));
  anm = AnmManager::newAnim(PlayerBeamN);
  AnmManager::UpdateChild(anm, p.x, p.y, 0, 0, 0, d, 1, 1, 1);
  dmg_src = Samus::instance->allocDmgSrc();
  Samus::instance->getDamageSource(dmg_src).Activate(
      pos, 3, 1, DamageSource::Type::POWER_BEAM, id);
}

void Bullet::Break() {
  int a = AnmManager::newAnim(PlayerBeamHitN);
  AnmManager::UpdateChild(a, pos.x, pos.y, 0, 0, 0, 0, 1, 1, 1);
  active = false;
  AnmManager::deleteAnim(anm);
  anm = -1;
  Samus::instance->getDamageSource(dmg_src).flags = 0;
}

void Bullet::Update() {
  if (!active)
    return;
  lifetime++;
  if (lifetime > BULLET_MAX_LIFE) {
    active = false;
    AnmManager::deleteAnim(anm);
    anm = -1;
    Samus::instance->getDamageSource(dmg_src).flags = 0;
  }
  pos += spd;
  AnmManager::UpdateChild(anm, pos.x, pos.y, 0, 0, 0, d, 1, 1, 1);
  Samus::instance->getDamageSource(dmg_src).pos = pos;
  Samus::instance->getDamageSource(dmg_src).spd = spd;
  // check hit with enemy

  // check hit with surface
  if (Collider::collideAt(pos)) {
    glm::vec2 sss = glm::normalize(spd);
    while (Collider::collideAt(pos))
      pos -= sss;
    Break();
  }
}

void Missile::Launch(const glm::vec2 &p, float angle, uint8_t flags) {
  lifetime = 0;
  this->angle = angle;
  this->flags = flags | 1;
  speed = 3.f;
  spd = glm::vec2(speed * cos(angle), speed * sin(angle));
  pos = p + 5.f * spd;
  anmID = AnmManager::newAnim(PlayerMissile);
  AnmManager::UpdateChild(anmID, pos.x, pos.y, 0, 0, 0, angle, 1, 1, 1);
  dmg_src = Samus::instance->allocDmgSrc();
  Samus::instance->getDamageSource(dmg_src).Activate(
      pos, 3, 1, DamageSource::Type::MISSILE, id);
}

void Missile::Break() {
  int a = AnmManager::newAnim(anmeffect_explosion);
  AnmManager::UpdateChild(a, pos.x, pos.y, 0, 0, 0, 0, 1, 1, 1);
  flags = 0;
  AnmManager::deleteAnim(anmID);
  anmID = -1;
  Samus::instance->getDamageSource(dmg_src).flags = 0;
}

void Missile::Update() {
  if (!(flags & 1))
    return;
  lifetime++;
  if (lifetime > BULLET_MAX_LIFE) {
    flags = 0;
    AnmManager::deleteAnim(anmID);
    anmID = -1;
    Samus::instance->getDamageSource(dmg_src).flags = 0;
  }
  pos += spd;
  Samus::instance->getDamageSource(dmg_src).pos = pos;
  Samus::instance->getDamageSource(dmg_src).spd = spd;
  AnmManager::UpdateChild(anmID, pos.x, pos.y, 0, 0, 0, angle, 1, 1, 1);
  if (lifetime > 10 && speed < 15.f) {
    speed += 0.5f;
    spd = {speed * cos(angle), speed * sin(angle)};
  }

  if (lifetime > 10 && lifetime % 8 == 0)
    AnmManager::UpdateChild(AnmManager::newAnim(anmeffects_missileSmoke),
                            pos.x - spd.x * 2, pos.y - spd.y * 2, 0, 0, 0,
                            angle, 1, 1, true);
  // check hit with enemy

  // check hit with surface
  if (Collider::collideAt(pos)) {
    glm::vec2 sss = glm::normalize(spd);
    while (Collider::collideAt(pos))
      pos -= sss;
    Break();
  }
}

void Bomb::Place(const glm::vec2 &p, uint8_t flags) {
  lifetime = 0;
  this->flags = flags | 1;
  pos = p;
  anmID = AnmManager::newAnim(PlayerMissile);
  AnmManager::UpdateChild(anmID, pos.x, pos.y, 0, 0, 0, 0, 1, 1, 1);
}

void Bomb::Update() {
  if (!isActive())
    return;
  lifetime++;

  // Depends on type of bomb (stored in flags)
  if (lifetime >= 60) {
    dmg_src = Samus::instance->allocDmgSrc();
    Samus::instance->getDamageSource(dmg_src).Activate(
        pos, 14, 1, DamageSource::Type::BOMB, id, 3);
    flags = 0;
    AnmManager::deleteAnim(anmID);
    // explosion anm
  }
}
