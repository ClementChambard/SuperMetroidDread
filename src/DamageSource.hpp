// Copyright 2023 Clément Chambard
#ifndef DAMAGESOURCE_H_
#define DAMAGESOURCE_H_

#include <glm/glm.hpp>

class DamageSource {
public:
  enum class Type : uint8_t {
    POWER_BEAM = 0,
    POWER_BEAM_CHARGE,
    POWER_BEAM_DIFFUSION,
    POWER_BEAM_DIFFUSION_EXPLODE,
    PLASMA_BEAM,
    PLASMA_BEAM_CHARGE,
    PLASMA_BEAM_DIFFUSION,
    PLASMA_BEAM_DIFFUSION_EXPLODE,
    WAVE_BEAM,
    WAVE_BEAM_CHARGE,
    WAVE_BEAM_DIFFUSION,
    WAVE_BEAM_DIFFUSION_EXPLODE,
    MISSILE,
    SUPER_MISSILE,
    ICE_MISSILE,
    STORM_MISSILE,
    BOMB,
    POWER_BOMB,
    GRAPPLE_BEAM,
    SCREW_ATTACK,
    SPEED_BOOST,
  };

  void Activate(glm::vec2 position, float size, int dmg, Type type,
                uint8_t obj_id, int ttl = 99999);

  bool active() const { return flags & 1; }
  bool isBeam() const { return type <= Type::PLASMA_BEAM_DIFFUSION_EXPLODE; }
  bool isMissile() const {
    return type >= Type::MISSILE && type <= Type::STORM_MISSILE;
  }

  bool intersectCircle(glm::vec2 pos, float size) const;
  bool intersectRectangle(glm::vec2 pos1, glm::vec2 pos2) const;

  void Draw() const;
  void hit(bool terrain = true);

  uint8_t flags = 0;
  glm::vec2 pos{};
  float size = 0.f;
  glm::vec2 spd{};
  int damage = 0;
  uint8_t obj_id;
  Type type;
  int ttl = 0;
};

#define MAX_DAMAGE_SOURCE 200

typedef DamageSource DamageSourcePool[MAX_DAMAGE_SOURCE];

#endif // DAMAGESOURCE_H_
