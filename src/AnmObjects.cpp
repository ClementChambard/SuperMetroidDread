#include "AnmObjects.h"
#include <math/Random.h>

static int s_blocbeam;
static int s_blocbomb;
static int s_blocpbomb;
static int s_blocscrew;
static int s_blocmissile;
static int s_bloccrumble;
static int s_blocspeed;
static int s_blocreform;
static int s_blocpiece1;
static int s_blocpiece2;
static int s_blocpiece3;
static int s_blocpiece4;

void AnmObjects::Init()
{
    int t = NSEngine::TextureManager::RegisterTexture("assets/textures/objects.png");
    s_blocbeam    = GS_ AddSprite(t,  0, 0,24,24);
    s_blocbomb    = GS_ AddSprite(t,  0,24,24,24);
    s_blocpbomb   = GS_ AddSprite(t, 24,24,24,24);
    s_blocscrew   = GS_ AddSprite(t, 48,24,24,24);
    s_blocmissile = GS_ AddSprite(t, 72, 0,24,24);
    s_bloccrumble = GS_ AddSprite(t, 24, 0,24,24);
    s_blocspeed   = GS_ AddSprite(t, 48, 0,24,24);
    s_blocreform  = GS_ AddSprite(t, 72,24,24,24);
    s_blocpiece1  = GS_ AddSprite(t, 96, 0,12,12);
    s_blocpiece2  = GS_ AddSprite(t,108, 0,12,12);
    s_blocpiece3  = GS_ AddSprite(t, 96,12,12,12);
    s_blocpiece4  = GS_ AddSprite(t,108,12,12,12);
}

static void anmobject_blocbreak(AnimScript* anm) {
    if(anm->frame == 0) {
        anm->set_layer(1);
        anm->alpha_time(15, 0, 0);

        anm->x_time(15, 7, Random::Floatm11()*3);
        anm->y_time_ext(15, 17, -0.3, 0, Random::Floatm11()*3+1);
        anm->set_rot(Random::Floatm11()*0.2, Random::Floatm11()*0.2, Random::Angle());
        anm->rot_vel(Random::Floatm11()*0.02, Random::Floatm11()*0.02, Random::Angle()/4.f);
        anm->set_scale(Random::Float01()*0.2+0.8,Random::Float01()*0.2+0.8);
        anm->scale_time(15, 4, Random::Float01()*0.1+0.6,Random::Float01()*0.1+0.6);
    }
    if (anm->frame == 15) anm->destroy();
}

static void anmobject_blocrespawn(AnimScript* anm) {
    if (anm->frame == 0) {
        anm->set_layer(1);
        anm->set_sprite(s_blocreform);
        anm->set_alpha(64);
        anm->alpha_time(13, 0, 255);
        //anm->set_scale(0.5, 0.5);
        //anm->zoom(0.5,0.5);
        //anm->set_pos()
        anm->scale_time(20, 0, 1, 1);
        //anm->zoom_time(30, 0, 1, 1);
    } else if (anm->frame == 13) {
        anm->alpha_time(7, 0, 0);
    }
    if (anm->frame == 20) anm->destroy();
}

static void anmobject_blocscr(AnimScript* anm) {
    if (anm->frame == 0) {
        anm->set_layer(1);
    }

    if (anm->interrupt == 1) {
        anm->set_alpha(0);
        AnmManager::anim(AnmManager::newAnim(anmobject_blocbreak, -1, false))->set_pos(anm->x_(1), anm->y_(1), 0)->set_sprite(s_blocpiece1);
        AnmManager::anim(AnmManager::newAnim(anmobject_blocbreak, -1, false))->set_pos(anm->x_(1), anm->y_(1), 0)->set_sprite(s_blocpiece2);
        AnmManager::anim(AnmManager::newAnim(anmobject_blocbreak, -1, false))->set_pos(anm->x_(1), anm->y_(1), 0)->set_sprite(s_blocpiece3);
        AnmManager::anim(AnmManager::newAnim(anmobject_blocbreak, -1, false))->set_pos(anm->x_(1), anm->y_(1), 0)->set_sprite(s_blocpiece4);
    } else if (anm->interrupt == 2) {
        anm->visible(true);
        anm->set_scale(0.5, 0.5);
        anm->scale_time(24, 0, 1, 1);
        anm->set_alpha(64);
        anm->alpha_time(24, 0, 255);
        anm->create_child(anmobject_blocrespawn);
    }
}

uint16_t anmobject_block(int type, int x, int y, int interrupt)
{
    auto anm = AnmManager::newAnim(anmobject_blocscr);

    if (type == 0) AnmManager::anim(anm)->set_sprite(s_blocbeam);
    else if (type == 1) {
        AnmManager::anim(anm)->set_sprite(s_bloccrumble);
        AnmManager::anim(anm)->set_color2(200,200,200);
    }
    else if (type == 2) {
        AnmManager::anim(anm)->set_sprite(s_blocmissile);
        AnmManager::anim(anm)->set_color2(240,128,128);
    }
    else if (type == 3) {
        AnmManager::anim(anm)->set_sprite(s_blocbomb);
        AnmManager::anim(anm)->set_color2(240,128,240);
    }
    else if (type == 4) {
        AnmManager::anim(anm)->set_sprite(s_blocpbomb);
        AnmManager::anim(anm)->set_color2(240,240,40);
    }
    else if (type == 5) {
        AnmManager::anim(anm)->set_sprite(s_blocscrew);
        AnmManager::anim(anm)->set_color2(40,240,240);
    }
    else if (type == 6) {
        AnmManager::anim(anm)->set_sprite(s_blocspeed);
        AnmManager::anim(anm)->set_color2(240,240,40);
    }
    return anm;
}
