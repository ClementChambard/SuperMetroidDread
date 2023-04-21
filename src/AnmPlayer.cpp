#include "AnmPlayer.h"
#include <NSEngine.h>

int plSprBeamN;
int plSprBeamC;
int plSprBeamFireN1;
int plSprBeamFireN2;
int plSprBeamFireN3;
int plSprBeamFireC1;
int plSprBeamFireC2;
int plSprBeamFireC3;
int plSprBeamFireS;
int plSprBeamHitN;
int plSprBeamHitC;
int plSprMissile;

int plSprAimRR;
int plSprAimRTR;
int plSprAimRBR;
int plSprAimRU;
int plSprAimLR;
int plSprAimLTR;
int plSprAimLBR;
int plSprAimLU;
int plSprLegs;

void AnmPlayer::Init()
{
    int tex = NSEngine::TextureManager::RegisterTexture("assets/textures/beams.png");
    plSprBeamFireN1 = GS_ AddSprite(tex, 0, 16, 32, 16);
    plSprBeamFireN2 = GS_ AddSprite(tex,32, 16, 32, 16);
    plSprBeamFireN3 = GS_ AddSprite(tex,64, 16, 32, 16);
    plSprBeamFireC1 = GS_ AddSprite(tex, 0, 32, 32, 16);
    plSprBeamFireC2 = GS_ AddSprite(tex,32, 32, 32, 16);
    plSprBeamFireC3 = GS_ AddSprite(tex,64, 32, 32, 16);
    plSprBeamFireS = GS_ AddSprite(tex, 0, 0, 48, 16);
    plSprBeamN = GS_ AddSprite(tex,48, 0, 48, 16);
    plSprBeamHitN = GS_ AddSprite(tex,96,0,16,16);
    plSprBeamC = 0;
    plSprBeamHitC = 0;
    plSprMissile = GS_ AddSprite(tex, 96, 16, 32, 16);

    tex = NSEngine::TextureManager::RegisterTexture("assets/textures/samus.png");
    plSprAimRR = GS_ AddSprite(tex, 0, 0, 24, 24);
    plSprAimRTR = GS_ AddSprite(tex, 48, 0, 24, 24);
    plSprAimRBR = GS_ AddSprite(tex, 24, 0, 24, 24);
    plSprAimRBR = GS_ AddSprite(tex, 72, 0, 24, 24);
    plSprAimLR = GS_ AddSprite(tex, 0, 24, 24, 24);
    plSprAimLTR = GS_ AddSprite(tex, 48, 24, 24, 24);
    plSprAimLBR = GS_ AddSprite(tex, 24, 24, 24, 24);
    plSprAimLBR = GS_ AddSprite(tex, 72, 24, 24, 24);
    plSprLegs = GS_ AddSprite(tex, 0, 48, 24, 24);
}

void PlayerMissile(AnimScript* anm)
{
    if (anm->frame == 0)
    {
        anm->set_layer(0);
        anm->set_sprite(plSprMissile);
    }
}

void PlayerBeamFireN(AnimScript* anm)
{
    if (anm->frame == 0)
    {
        anm->set_alpha(224);
        anm->alpha_time(6,0,64);
        anm->set_layer(0);
        anm->set_pos(10, 0, 0);
        anm->set_sprite(plSprBeamFireN1);
    }
    else if (anm->frame == 2) anm->set_sprite(plSprBeamFireN2);
    else if (anm->frame == 4) anm->set_sprite(plSprBeamFireN3);
    else if (anm->frame == 6) anm->destroy();
}

void PlayerBeamFireC(AnimScript* anm)
{
    if (anm->frame == 0)
    {
        anm->set_alpha(224);
        anm->alpha_time(6,0,64);
        anm->set_layer(0);
        anm->set_pos(10, 0, 0);
        anm->set_sprite(plSprBeamFireC1);
    }
    else if (anm->frame == 2) anm->set_sprite(plSprBeamFireC2);
    else if (anm->frame == 4) anm->set_sprite(plSprBeamFireC3);
    else if (anm->frame == 6) anm->destroy();
}

void PlayerBeamFireS(AnimScript* anm)
{
    if (anm->frame == 0)
    {
        anm->set_alpha(224);
        anm->alpha_time(6,0,0);
        anm->set_layer(0);
        anm->set_rot(0, 0, -3.2425927/2);
        anm->set_pos(8,0,0);
        anm->set_sprite(plSprBeamFireS);
        anm->set_scale(0.4f,0.4f);
        anm->scale_time(6,4,1.3f,1.3f);
    }
    else if (anm->frame == 6) anm->destroy();
}

#include <math/Random.h>
void PlayerBeamN(AnimScript* anm)
{
    if (anm->frame == 0)
    {
        anm->set_alpha(0);
        anm->alpha_time(5,0,240);
        anm->set_pos(-16, 0, 0);
        anm->set_layer(0);
        anm->set_sprite(plSprBeamN);
        anm->set_scale(0.2f, .8f);
        anm->scale_time(6, 0, 1.3f, .8f);
    }
}
void PlayerBeamHitN(AnimScript* anm)
{
    if (anm->frame == 0)
    {
        anm->set_layer(0);
        anm->set_sprite(plSprBeamHitN);
        anm->alpha_time(15,0,0);
        anm->set_scale(.8f,.8f);
        anm->scale_time(15,1,1.5f,1.5f);
        anm->rot_vel(0,0,1);
        anm->set_rot(0,0,Random::Angle());
    }
    else if (anm->frame == 15) anm->destroy();
}

int PlayerSprite() { return plSprAimRR; }
