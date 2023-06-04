// Copyright 2023 Clément Chambard
#ifndef SAMUS_INCLUDED_H
#define SAMUS_INCLUDED_H

#define COL_SIZE_NORMAL                                                        \
  { 16, 47 }
#define COL_SIZE_CROUCH                                                        \
  { 16, 32 }
#define COL_SIZE_MORPH                                                         \
  { 16, 20 }

#include "./DamageSource.hpp"
#include "Bullet.h"
#include "Collider.h"

enum class PlayerState {
  FREE = 0,
  IDLE,
  CROUCHING,
  WALKING,
  AIRNORMAL,
  SPINJUMP,
  SLIDING,
  SLIDE_AIR_BOOST,
  MORPH,
  MORPH_AIR,
};

struct PlayerInputs {
  struct InputButton {
    uint8_t down : 1;
    uint8_t pressed : 1;
    uint8_t released : 1;
    uint8_t unused : 5;
    operator bool() { return down; }
    void process(int keyCode);
  };
  InputButton dirUp{};
  InputButton dirDo{};
  InputButton dirLe{};
  InputButton dirRi{};
  InputButton dirAu{};
  InputButton dirAd{};
  InputButton jump{};
  InputButton shoot{};
  InputButton morph{};

  void process();
};

union AimDir {
  AimDir(uint8_t d) { dat = d; }
  struct {
    uint8_t forward : 1;
    uint8_t forward_up : 1;
    uint8_t up : 1;
    uint8_t backward_up : 1;
    uint8_t backward : 1;
    uint8_t backward_down : 1;
    uint8_t down : 1;
    uint8_t forward_down : 1;
  } bits;
  uint8_t dat;
  static constexpr float SHOOT_ANGLES[8] = {0.0f,    0.7854f, 1.5708f, 2.3562f,
                                            3.1416f, 3.927f,  4.7124f, 5.4978f};
  operator uint8_t() { return dat; }
  float getAngle() {
    for (int i = 0; i < 8; i++)
      if ((dat >> i) & 1)
        return SHOOT_ANGLES[i];
    return 0.f;
  }
};
#define AimDir_FORWARD 0b00000001
#define AimDir_FORWARD_UP 0b00000010
#define AimDir_UP 0b00000100
#define AimDir_BACKWARD_UP 0b00001000
#define AimDir_BACKWARD 0b00010000
#define AimDir_BACKWARD_DOWN 0b00100000
#define AimDir_DOWN 0b01000000
#define AimDir_FORWARD_DOWN 0b10000000
#define AimDir_ALL 0b11111111

class Samus {
public:
  Samus();
  ~Samus();

  void Update();
  void Draw();

  static Samus *instance;

  Bullet *getBullets() { return &bullet_pool[0]; }

  void setState(PlayerState s) {
    state = s;
    state_timer = 0;
    if (state == PlayerState::SLIDING ||
        state == PlayerState::SLIDE_AIR_BOOST || state == PlayerState::MORPH ||
        state == PlayerState::MORPH_AIR) {
      col.setSize(COL_SIZE_MORPH);
    } else if (state == PlayerState::CROUCHING ||
               state == PlayerState::SPINJUMP) {
      col.setSize(COL_SIZE_CROUCH);
    } else {
      col.setSize(COL_SIZE_NORMAL);
    }
  }

  void setSpeed(glm::vec2 speed) {
    spd = speed;
    col.speedFraction = {0, 0};
  }
  void setSpeed(float x, float y) { setSpeed({x, y}); }
  void setSpeedX(float x) { setSpeed({x, spd.y}); }
  void setSpeedY(float y) { setSpeed({spd.x, y}); }
  void accel(glm::vec2 accel, glm::vec2 maxSpd) {
    spd += accel;
    if (maxSpd.x > 0 && abs(spd.x) > maxSpd.x)
      spd.x = maxSpd.x * spd.x / abs(spd.x);
    if (maxSpd.y > 0 && abs(spd.y) > maxSpd.y)
      spd.y = maxSpd.y * spd.y / abs(spd.y);
  }
  void accelX(float x, float maxX) { accel({x, 0}, {maxX, 0}); }
  void accelY(float y, float maxY) { accel({0, y}, {0, maxY}); }
  void applyAccelX(float xAccel, float xSpdMax, float xFriction,
                   bool turn_add = false);
  void applyAccelY(float yAccel, float ySpdMax, float yFriction,
                   bool turn_add = false);
  void allowMovement(bool grounded = false);
  void allowShoot();
  void allowPlaceBomb();
  void allowCharge();
  void allowAim(AimDir allowedAim = AimDir_ALL,
                AimDir defaultAim = AimDir_FORWARD);

  Collider col{{0, 0}, {0, 0}};
  glm::vec2 pos() { return col.bottom(); }
  glm::vec2 spd = {0, 0};
  bool faceR = true;
  uint32_t state_timer = 0;
  PlayerState state = PlayerState::IDLE;
  PlayerInputs inputs{};

  uint32_t noMovementControlTimer = 0;
  uint8_t missileCooldown = 0;
  /*  HAS ABILITY  */
  uint32_t hasCharge : 1;
  uint32_t hasDiffuse : 1;
  uint32_t hasSpazer : 1;
  uint32_t hasPlasma : 1;
  uint32_t hasWave : 1;
  uint32_t hasHyper : 1;
  uint32_t hasGrapple : 1;
  uint32_t hasSuperM : 1;
  uint32_t hasIceM : 1;
  uint32_t hasStormM : 1;
  uint32_t hasPhanom : 1;
  uint32_t hasFlash : 1;
  uint32_t hasPulse : 1;
  uint32_t hasVaria : 1;
  uint32_t hasGravity : 1;
  uint32_t hasMetroid : 1;
  uint32_t hasMorph : 1;
  uint32_t hasBombs : 1;
  uint32_t hasCBombs : 1;
  uint32_t hasPBombs : 1;
  uint32_t hasSpider : 1;
  uint32_t hasSpeefB : 1;
  uint32_t hasSpinBo : 1;
  uint32_t hasSpaceJ : 1;
  uint32_t hasScrew : 1;
  uint32_t has__unused : 7;

  uint16_t missileCount = 15;
  uint8_t pbombCount = 2;
  bool usedSpinboost = false;

  /*  SHOOT */
  AimDir aim = 0;
  char chargeCtr = 0;
  char chargeCtr2 = 0;
  unsigned int curBullet = 0;
  unsigned int curMissile = 0;
  Bullet bullet_pool[MAX_BULLETS];
  Missile missile_pool[MAX_MISSILE];
  Bomb bomb_pool[7];
  DamageSourcePool damage_sources;
  glm::vec2 shootPoint;
  void Shoot(bool charge, bool missile);

  /*  ANIMS  */
  static constexpr unsigned int MAX_ANIM = 10;
  int anims[MAX_ANIM];
  // 0 = bottom, 1 = top, 2 = missile cap, 3, 4 = shoot
  // anim / charge
  glm::vec2 anmPositions[MAX_ANIM];
  float anmRotations[MAX_ANIM];
  float anmScales[MAX_ANIM];
  void SetAnim(int id, int anm);
  void SetAnim(int id, int anm, glm::vec2 relPos, float rot = 0,
               float scale = 1);
  DamageSource &getDamageSource(int id) { return damage_sources[id]; }
  int allocDmgSrc() {
    static int cur = 0;
    int i = 0;
    while (i++ < MAX_DAMAGE_SOURCE && damage_sources[cur].active())
      cur = (cur + 1) % MAX_DAMAGE_SOURCE;
    return cur;
  }
  void UpdateAnims();
};

#endif
