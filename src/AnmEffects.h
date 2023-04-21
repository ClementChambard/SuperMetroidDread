#ifndef ANMEFFECTS_H_
#define ANMEFFECTS_H_

#include "AnmManagerOld/AnmManager.h"

class AnmEffects {
    public:
        static void Init();
};

extern void anmeffects_doubleJump(AnimScript* anm);
extern void anmeffects_wallJumpL(AnimScript* anm);
extern void anmeffects_wallJumpR(AnimScript* anm);
extern void anmeffects_missileSmoke(AnimScript* anm);
extern void anmeffect_explosion(AnimScript* anm);
extern void anmeffect_particleBurst(AnimScript* anm);

#endif // ANMEFFECTS_H_
