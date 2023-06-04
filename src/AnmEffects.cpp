// Copyright 2023 Clément Chambard

#include "AnmEffects.h"
#include <math/Random.h>

static int s_circle1;
static int s_circle2;
static int s_circle3;
static int s_circle4;
static int s_spark11;
static int s_spark12;
static int s_spark13;
static int s_spark21;
static int s_spark22;
static int s_spark23;
static int s_smoke1;
static int s_smoke2;
static int s_smoke3;
static int s_smoke4;
static int s_smoke5;
static int s_smoke6;

void AnmEffects::Init() {
  int t =
      NSEngine::TextureManager::RegisterTexture("assets/textures/effects.png");
  s_circle1 = GS_ AddSprite(t, 0, 0, 48, 48);
  s_circle2 = GS_ AddSprite(t, 48, 0, 48, 48);
  s_circle3 = GS_ AddSprite(t, 96, 0, 48, 48);
  s_circle4 = GS_ AddSprite(t, 144, 0, 48, 48);
  s_spark11 = GS_ AddSprite(t, 192, 0, 16, 16);
  s_spark12 = GS_ AddSprite(t, 192, 16, 16, 16);
  s_spark13 = GS_ AddSprite(t, 192, 32, 16, 16);
  s_spark21 = GS_ AddSprite(t, 208, 0, 16, 16);
  s_spark22 = GS_ AddSprite(t, 208, 16, 16, 16);
  s_spark23 = GS_ AddSprite(t, 208, 32, 16, 16);
  s_smoke1 = GS_ AddSprite(t, 224, 32, 16, 16);
  s_smoke2 = GS_ AddSprite(t, 240, 32, 16, 16);
  s_smoke3 = GS_ AddSprite(t, 256, 32, 16, 16);
  s_smoke4 = GS_ AddSprite(t, 224, 0, 32, 32);
  s_smoke5 = GS_ AddSprite(t, 256, 0, 32, 32);
  s_smoke6 = GS_ AddSprite(t, 288, 0, 32, 32);
}

static void anmeffects_spark1(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->set_alpha(128);
    anm->alpha_time(7, 0, 32);
    anm->set_layer(0);
    anm->set_sprite(s_spark11);
  } else if (anm->frame == 2) {
    anm->set_sprite(s_spark12);
  } else if (anm->frame == 4) {
    anm->set_sprite(s_spark13);
  } else if (anm->frame == 6) {
    anm->destroy();
  }
}

static void anmeffects_spark2(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->set_alpha(128);
    anm->alpha_time(7, 0, 32);
    anm->set_layer(0);
    anm->set_sprite(s_spark21);
  } else if (anm->frame == 2) {
    anm->set_sprite(s_spark22);
  } else if (anm->frame == 4) {
    anm->set_sprite(s_spark23);
  } else if (anm->frame == 6) {
    anm->destroy();
  }
}

static void anmeffects_doubleJumpCircle1(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->set_alpha(128);
    anm->alpha_time(7, 0, 32);
    anm->set_layer(0);
    anm->set_rot(PI1_2, 0, Random::Angle());
    anm->rot_vel(0, 0, 0.1);
    anm->set_pos(0, -5, 0);
    anm->pos_time(8, 0, 0, -10, 0);
    anm->set_color(255, 208, 255);
    anm->scale_time(8, 0, 1.2, 1.2);
    anm->set_sprite(s_circle1);
  } else if (anm->frame == 2) {
    anm->set_sprite(s_circle2);
  } else if (anm->frame == 4) {
    anm->set_sprite(s_circle3);
  } else if (anm->frame == 6) {
    anm->set_sprite(s_circle4);
  } else if (anm->frame == 8) {
    anm->destroy();
  }
}

static void anmeffects_doubleJumpCircle2(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->set_alpha(128);
    anm->alpha_time(7, 0, 32);
    anm->set_layer(0);
    anm->set_rot(PI1_2, 0, Random::Angle());
    anm->rot_vel(0, 0, 0.1);
    anm->set_pos(0, -10, 0);
    anm->pos_time(8, 0, 0, -15, 0);
    anm->set_color(160, 255, 255);
    anm->set_scale(1.4, 1.4);
    anm->scale_time(8, 0, 1.8, 1.8);
    anm->set_sprite(s_circle1);
  } else if (anm->frame == 2) {
    anm->set_sprite(s_circle2);
  } else if (anm->frame == 4) {
    anm->set_sprite(s_circle3);
  } else if (anm->frame == 6) {
    anm->set_sprite(s_circle4);
  } else if (anm->frame == 8) {
    anm->destroy();
  }
}

void anmeffects_doubleJump(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->visible(false);
    anm->create_child(anmeffects_doubleJumpCircle1);
    anm->create_child(anmeffects_doubleJumpCircle2);
    auto i = anm->create_child(anmeffects_spark1, false);
    AnmManager::anim(i)
        ->set_pos(-9, 0, 0)
        ->set_rot(0, 0, 1)
        ->set_scale(0.5, 0.8)
        ->pos_time(6, 0, -10 + 11 * sin(-1), 11 * cos(-1), 0);
    i = anm->create_child(anmeffects_spark2, false);
    AnmManager::anim(i)
        ->set_pos(-5, 2, 0.1)
        ->set_rot(0, 0, 0.5)
        ->set_scale(0.7, 0.9)
        ->pos_time(6, 0, -5 + 10 * sin(-0.5), 2 + 10 * cos(-0.5), 0.1);
    i = anm->create_child(anmeffects_spark2, false);
    AnmManager::anim(i)
        ->set_pos(0, 5, 0.2)
        ->set_rot(0, 0, 0)
        ->set_scale(1, 1)
        ->pos_time(6, 0, 0, 15, 0.2);
    i = anm->create_child(anmeffects_spark1, false);
    AnmManager::anim(i)
        ->set_pos(5, 2, 0.1)
        ->set_rot(0, 0, -0.5)
        ->set_scale(0.7, 0.9)
        ->pos_time(6, 0, 5 + 10 * sin(0.5), 2 + 10 * cos(0.5), 0.1);
    i = anm->create_child(anmeffects_spark2, false);
    AnmManager::anim(i)
        ->set_pos(9, 0, 0)
        ->set_rot(0, 0, -1)
        ->set_scale(0.5, 0.8)
        ->pos_time(6, 0, 10 + 11 * sin(1), 11 * cos(1), 0);
  }
  if (anm->frame == 8)
    anm->destroy();
}

void anmeffects_wallJumpL(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->visible(false);
    auto i = anm->create_child(anmeffects_doubleJumpCircle1, true);
    AnmManager::anim(i)->set_rot(0, PI1_2, 0)->pos_time(0, 0, 0, 0, 0);
    i = anm->create_child(anmeffects_doubleJumpCircle2, true);
    AnmManager::anim(i)->set_rot(0, PI1_2, 0)->pos_time(0, 0, 0, 0, 0);
    i = anm->create_child(anmeffects_spark2, false);
    AnmManager::anim(i)
        ->set_pos(5, 5, 0.2)
        ->set_rot(0, 0, -.2)
        ->set_scale(1, 1)
        ->pos_time(6, 0, 0, 15, 0.2);
    i = anm->create_child(anmeffects_spark1, false);
    AnmManager::anim(i)
        ->set_pos(9, 2, 0.1)
        ->set_rot(0, 0, -0.6)
        ->set_scale(0.7, 0.9)
        ->pos_time(6, 0, 5 + 10 * sin(0.5), 2 + 10 * cos(0.5), 0.1);
    i = anm->create_child(anmeffects_spark2, false);
    AnmManager::anim(i)
        ->set_pos(11, 0, 0)
        ->set_rot(0, 0, -1.2)
        ->set_scale(0.5, 0.8)
        ->pos_time(6, 0, 10 + 11 * sin(1), 11 * cos(1), 0);
  }
  if (anm->frame == 8)
    anm->destroy();
}
void anmeffects_wallJumpR(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->visible(false);
    auto i = anm->create_child(anmeffects_doubleJumpCircle1, true);
    AnmManager::anim(i)->set_rot(0, -PI1_2, 0)->pos_time(0, 0, 0, 0, 0);
    i = anm->create_child(anmeffects_doubleJumpCircle2, true);
    AnmManager::anim(i)->set_rot(0, -PI1_2, 0)->pos_time(0, 0, 0, 0, 0);
    i = anm->create_child(anmeffects_spark1, false);
    AnmManager::anim(i)
        ->set_pos(-11, 0, 0)
        ->set_rot(0, 0, 1.2)
        ->set_scale(0.5, 0.8)
        ->pos_time(6, 0, -10 + 11 * sin(-1), 11 * cos(-1), 0);
    i = anm->create_child(anmeffects_spark2, false);
    AnmManager::anim(i)
        ->set_pos(-9, 2, 0.1)
        ->set_rot(0, 0, 0.6)
        ->set_scale(0.7, 0.9)
        ->pos_time(6, 0, -5 + 10 * sin(-0.5), 2 + 10 * cos(-0.5), 0.1);
    i = anm->create_child(anmeffects_spark2, false);
    AnmManager::anim(i)
        ->set_pos(0, 5, 0.2)
        ->set_rot(0, 0, 0.2)
        ->set_scale(1, 1)
        ->pos_time(6, 0, 0, 15, 0.2);
  }
  if (anm->frame == 8)
    anm->destroy();
}

void anmeffects_missileSmoke(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->set_layer(1);
    anm->set_sprite(s_smoke1);
    anm->set_color(32, 32, 32);
    anm->set_alpha(255);
    anm->alpha_time(12, 0, 0);
    anm->set_scale(5, 1);
    anm->scale_time(12, 1, 6, 0.3);
  }
  if (anm->frame == 4)
    anm->set_sprite(s_smoke2);
  if (anm->frame == 8)
    anm->set_sprite(s_smoke3);
  if (anm->frame == 12)
    anm->destroy();
}

void explosion_part(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->set_layer(1);
    anm->set_sprite(s_smoke1);
    anm->set_rot(0, 0, Random::Angle());
    anm->rot_vel(0, 0, Random::Angle() / 20.f);
    anm->set_color(255, 64, 32);
    anm->alpha_time(12, 0, 128);
    anm->scale_time(12, 0, Random::Float01() + 1, Random::Float01() + 1);
    anm->color_time(6, 0, 100, 80, 40);
    anm->set_blendmode(1);
    anm->set_pos(Random::Floatm11() * 12, Random::Floatm11() * 12, 0);
  }
  if (anm->frame == 4)
    anm->set_sprite(s_smoke2);
  if (anm->frame == 6)
    anm->color_time(6, 0, 64, 64, 64);
  if (anm->frame == 8)
    anm->set_sprite(s_smoke3);
  if (anm->frame == 12)
    anm->destroy();
}

void explosion_part2(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->set_layer(1);
    anm->set_sprite(s_smoke4);
    anm->set_rot(0, 0, Random::Angle());
    anm->rot_vel(0, 0, Random::Angle() / 40.f);
    anm->set_color(255, 64, 32);
    anm->alpha_time(12, 0, 128);
    anm->scale_time(12, 0, Random::Float01() + 1, Random::Float01() + 1);
    anm->color_time(6, 0, 100, 80, 40);
    anm->set_blendmode(1);
    anm->set_pos(Random::Floatm11() * 12, Random::Floatm11() * 12, 0);
  }
  if (anm->frame == 5)
    anm->set_sprite(s_smoke5);
  if (anm->frame == 6)
    anm->color_time(8, 0, 64, 64, 64);
  if (anm->frame == 10)
    anm->set_sprite(s_smoke6);
  if (anm->frame == 14)
    anm->destroy();
}

void particleBurstPart(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->special_draw(6);
    anm->set_blendmode(1);
    anm->set_color(255, 144, 32);
    anm->set_color2(255, 144, 32);
    float a = Random::Angle();
    float r = Random::Floatm11() * 24;
    anm->pos_time(5, 4, r * cos(a), r * sin(a), Random::Floatm11() * 24);
    anm->alpha_time(5, 6, 128);
    anm->set_alpha2(255);
    anm->alpha2_time(5, 6, 128);
  }
  if (anm->frame == 5)
    anm->destroy();
}

void anmeffect_particleBurst(AnimScript *anm) {
  if (anm->frame == 0) {
    for (int i = 0; i < 10; i++)
      anm->create_child(particleBurstPart);
  }
  if (anm->frame == 2) {
    for (int i = 0; i < 10; i++)
      anm->create_child(particleBurstPart);
  }
  if (anm->frame == 4) {
    for (int i = 0; i < 10; i++)
      anm->create_child(particleBurstPart);
  }
  if (anm->frame == 10)
    anm->destroy();
}

void anmeffect_particleBurst45Deg(AnimScript *anm) {}

void anmeffect_explosion(AnimScript *anm) {
  if (anm->frame == 0) {
    anm->visible(false);
    anm->create_child(explosion_part2);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part2);
    anm->create_child(explosion_part2);
    anm->create_child(explosion_part2);
  }
  if (anm->frame == 2) {
    anm->create_child(explosion_part);
    anm->create_child(explosion_part2);
    anm->create_child(explosion_part2);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
  }
  if (anm->frame == 4) {
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
  }
  if (anm->frame == 6) {
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
    anm->create_child(explosion_part);
  }
  if (anm->frame == 20)
    anm->destroy();
}
