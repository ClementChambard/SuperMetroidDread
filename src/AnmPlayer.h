// Copyright 2023 Clément Chambard
#ifndef ANMPLAYER
#define ANMPLAYER

#include "AnmManagerOld/AnmManager.h"

class AnmPlayer {
public:
  static void Init();
};

void PlayerBeamFireN(AnimScript *anm);
void PlayerBeamFireC(AnimScript *anm);
void PlayerBeamFireS(AnimScript *anm);
void PlayerBeamN(AnimScript *anm);
void PlayerBeamC(AnimScript *anm);
void PlayerBeamHitN(AnimScript *anm);
void PlayerBeamHitC(AnimScript *anm);
void PlayerMissile(AnimScript *anm);

void PlayerSpr();

#endif // ANMPLAYER
