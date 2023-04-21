#include "Samus.h"
#include "AnmEffects.h"
#include "Area.h"
#include "Camera.h"
#include <InputManager.h>
#include <NSEngine.h>

#define RUN_SPEED 3.6
#define RUN_ACCEL 0.24
#define GROUND_FRICTION 0.6
#define JUMP_FORCE 7
#define SPIN_JUMP_SPEED_X 2.9
#define GRAVITY -0.25
#define AIR_SPEED_Y 10
#define AIR_FRICTION_Y 0
// #define AIR_SPEED_X 3
// #define AIR_ACCEL_X 3
// #define AIR_FRICTION_X 0.2
#define AIR_SPEED_X 2.064
#define AIR_ACCEL_X 0.516
#define AIR_FRICTION_X 0.2064
#define MORPH_SPEED 2.4
#define MORPH_ACCEL 0.1
#define MORPH_FRICTION 0.1
#define MORPH_JUMP_FORCE 6

int getAim(bool l, bool r, bool u, bool d, bool au, bool ad);

Samus* Samus::instance = nullptr;

#include "Text/SpriteFont.hpp"
static SpriteFont* font;

Samus::Samus()
{
    font = new SpriteFont("assets/font.fnt");
    // TODO : get starting position from save file
    glm::vec2 StartPos = glm::vec2(-3168, -624);
    col.setPos(StartPos).setSize(COL_SIZE_NORMAL);
    for (int i = 0; i < MAX_ANIM; i++)
        anims[i] = -1;
    instance = this;

    for (uint8_t i = 0; i < MAX_BULLETS; i++)
        bullet_pool[i].id = i;
    for (uint8_t i = 0; i < MAX_MISSILE; i++)
        missile_pool[i].id = i;
    for (uint8_t i = 0; i < 7; i++)
        bomb_pool[i].id = i;

    hasCharge = 0b1;
    hasDiffuse = 0b1;
    hasSpazer = 0b1;
    hasPlasma = 0b1;
    hasWave = 0b1;
    hasHyper = 0b1;
    hasGrapple = 0b1;
    hasSuperM = 0b1;
    hasIceM = 0b1;
    hasStormM = 0b1;
    hasPhanom = 0b1;
    hasFlash = 0b1;
    hasPulse = 0b1;
    hasVaria = 0b1;
    hasGravity = 0b1;
    hasMetroid = 0b1;
    hasMorph = 0b1;
    hasBombs = 0b1;
    hasCBombs = 0b1;
    hasPBombs = 0b1;
    hasSpider = 0b1;
    hasSpeefB = 0b1;
    hasSpinBo = 0b1;
    hasSpaceJ = 0b1;
    hasScrew = 0b1;
    has__unused = 0b0000000;
}

Samus::~Samus() { }

void Samus::applyAccelX(float xAccel, float xSpdMax, float xFriction, bool turn_add)
{
    if (abs(spd.x) > xSpdMax)
        xSpdMax = abs(spd.x) - xFriction / 2.f;
    if (xAccel) {
        spd.x += xAccel;
        if (turn_add && spd.x * xAccel < 0)
            spd.x -= xFriction * (abs(spd.x) / spd.x);
        if (spd.x > xSpdMax)
            spd.x = xSpdMax;
        if (spd.x < -xSpdMax)
            spd.x = -xSpdMax;
    } else if (xFriction) {
        if (spd.x > xFriction)
            spd.x -= xFriction;
        else if (spd.x < -xFriction)
            spd.x += xFriction;
        else
            spd.x = 0;
    }
}

void Samus::applyAccelY(float yAccel, float ySpdMax, float yFriction, bool turn_add)
{
    if (abs(spd.y) > ySpdMax)
        ySpdMax = abs(spd.y) - yFriction / 2.f;
    if (yAccel) {
        spd.y += yAccel;
        if (turn_add && spd.y * yAccel < 0)
            spd.y -= yFriction * (abs(spd.y) / spd.y);
        if (spd.y > ySpdMax)
            spd.y = ySpdMax;
        if (spd.y < -ySpdMax)
            spd.y = -ySpdMax;
    } else if (yFriction) {
        if (spd.y > yFriction)
            spd.y -= yFriction;
        else if (spd.y < -yFriction)
            spd.y += yFriction;
        else
            spd.y = 0;
    }
}

void playerState_Free(Samus* s)
{
    s->setSpeedY(AIR_SPEED_X * (s->inputs.dirUp - s->inputs.dirDo));
    s->setSpeedX(AIR_SPEED_X * (s->inputs.dirRi - s->inputs.dirLe));

    if (Inputs::Keyboard().Pressed(NSK_f))
        s->setState(PlayerState::IDLE);

    s->allowMovement();
    s->allowAim();
    s->allowShoot();
}

void playerState_Idle(Samus* s)
{
    // do things when samus is idle
    s->applyAccelX(0, RUN_SPEED, GROUND_FRICTION);
    if (s->inputs.jump.pressed) {
        s->setSpeedY(JUMP_FORCE);
        s->setState(PlayerState::AIRNORMAL);
        return;
    }

    if (Inputs::Keyboard().Pressed(NSK_f))
        s->setState(PlayerState::FREE);
    if (s->inputs.morph.pressed && s->hasMorph)
        s->setState(PlayerState::MORPH);

    if (!s->col.isGrounded()) {
        s->setState(PlayerState::AIRNORMAL);
        return;
    }

    // state interrupt
    if (s->inputs.dirLe || s->inputs.dirRi)
        s->setState(PlayerState::WALKING);
    else if (s->inputs.dirDo.pressed)
        s->setState(PlayerState::CROUCHING);
    else if (s->col.isHardSlopeBelow())
        s->setState(PlayerState::SLIDING);

    s->allowMovement(true);
    s->allowAim(AimDir_ALL & (~AimDir_DOWN));
    s->allowShoot();
}

void playerState_Crouching(Samus* s)
{
    // do things when samus is crouching
    s->applyAccelX(0, RUN_SPEED, GROUND_FRICTION);
    if (s->inputs.jump.pressed) {
        s->setSpeedY(JUMP_FORCE);
        s->setState(PlayerState::AIRNORMAL);
        return;
    }

    // state interrupt
    if (!s->col.isGrounded()) {
        s->setState(PlayerState::AIRNORMAL);
        return;
    }
    if (s->inputs.dirUp.pressed)
        s->setState(PlayerState::IDLE);
    if (s->hasMorph && (s->inputs.dirDo.pressed || s->inputs.morph.pressed && s->hasMorph))
        s->setState(PlayerState::MORPH);
    else if (s->inputs.dirLe || s->inputs.dirRi)
        s->setState(PlayerState::WALKING);
    else if (s->col.isHardSlopeBelow())
        s->setState(PlayerState::SLIDING);

    s->allowMovement(true);
    s->allowAim(AimDir_ALL & (~AimDir_DOWN));
    s->allowShoot();
}

static int ystart = 0;

void playerState_Airnormal(Samus* s)
{
    if (s->state_timer == 1) {
        ystart = s->col.pos.y - s->spd.y;
    }

    if (s->state_timer > 1)
        s->applyAccelY(GRAVITY, AIR_SPEED_Y, AIR_FRICTION_Y);
    s->applyAccelX(AIR_ACCEL_X * (s->inputs.dirRi - s->inputs.dirLe), AIR_SPEED_X, AIR_FRICTION_X);

    if (s->inputs.morph.pressed && s->hasMorph)
        s->setState(PlayerState::MORPH_AIR);
    if (s->col.isGrounded() && s->state_timer > 1) {
        s->setState(PlayerState::IDLE);
        s->usedSpinboost = false;
    } else if (s->inputs.jump.pressed)
        s->setState(PlayerState::SPINJUMP);
    s->allowMovement();
    s->allowAim();
    s->allowShoot();
}

void playerState_Spinjump(Samus* s)
{
    if (!s->noMovementControlTimer) {
        if (s->inputs.dirLe)
            s->faceR = false;
        if (s->inputs.dirRi)
            s->faceR = true;
    }
    s->applyAccelX(SPIN_JUMP_SPEED_X * (s->faceR ? 1 : -1), SPIN_JUMP_SPEED_X, abs(s->spd.x) > SPIN_JUMP_SPEED_X ? AIR_FRICTION_X : 0);
    if (s->state_timer > 1)
        s->applyAccelY(GRAVITY, AIR_SPEED_Y, AIR_FRICTION_Y);

    if (s->inputs.morph.pressed && s->hasMorph)
        s->setState(PlayerState::MORPH_AIR);
    if (s->col.isGrounded() && s->state_timer > 1) {
        s->setState(PlayerState::IDLE);
        s->usedSpinboost = false;
    }

    if (s->inputs.jump.pressed && (s->hasSpaceJ || (s->hasSpinBo && !s->usedSpinboost)) && s->spd.y < 0) {
        s->usedSpinboost = true;
        s->setSpeedY(JUMP_FORCE);
        AnmManager::UpdateChild(AnmManager::newAnim(anmeffects_doubleJump), s->col.bottom().x, s->col.bottom().y + 5, 0, 0, 0, 0, 1, 1, true);
    } else if (s->inputs.jump.pressed && s->col.isWallRight() && s->faceR) {
        s->setSpeedY(JUMP_FORCE);
        s->faceR = false;
        s->noMovementControlTimer = 29;
        AnmManager::UpdateChild(AnmManager::newAnim(anmeffects_wallJumpR), s->col.right().x, s->col.right().y + 5, 0, 0, 0, 0, 1, 1, true);
    } else if (s->inputs.jump.pressed && s->col.isWallLeft() && !s->faceR) {
        s->setSpeedY(JUMP_FORCE);
        s->faceR = true;
        s->noMovementControlTimer = 29;
        AnmManager::UpdateChild(AnmManager::newAnim(anmeffects_wallJumpL), s->col.left().x, s->col.left().y + 5, 0, 0, 0, 0, 1, 1, true);
    }

    s->allowMovement();
    s->allowCharge();

    if (s->inputs.shoot.pressed || s->inputs.shoot.released) {
        s->setState(PlayerState::AIRNORMAL);
        s->allowAim();
        s->allowShoot();
    }
    if (s->hasScrew) {
        int ds = s->allocDmgSrc();
        s->getDamageSource(ds).Activate(s->col.center(), 24, 1000, DamageSource::Type::SCREW_ATTACK, 255, 1);
    }
}

void playerState_Walk(Samus* s)
{
    s->applyAccelY(GRAVITY, AIR_SPEED_Y, AIR_FRICTION_Y);
    s->applyAccelX(RUN_ACCEL * (s->inputs.dirRi - s->inputs.dirLe), RUN_SPEED, GROUND_FRICTION, true);

    if (s->inputs.jump.pressed) {
        s->setSpeedY(JUMP_FORCE);
        s->setState(PlayerState::SPINJUMP);
        return;
    }

    if (s->inputs.dirDo)
        s->setState(PlayerState::CROUCHING);
    else if (s->inputs.morph.pressed)
        s->setState(PlayerState::SLIDING);
    else if (!s->inputs.dirLe && !s->inputs.dirRi)
        s->setState(PlayerState::IDLE);
    else if (!s->col.isGrounded())
        s->setState(PlayerState::AIRNORMAL);
    else if (s->col.isHardSlopeBelow())
        s->setState(PlayerState::SLIDING);

    s->allowMovement(true);
    s->allowAim(AimDir_ALL & (~AimDir_DOWN));
    s->allowShoot();
}

void playerState_Sliding(Samus* s)
{
    if (s->col.isHardSlopeL())
        s->faceR = false;
    if (s->col.isHardSlopeR())
        s->faceR = true;
    s->applyAccelX(s->faceR ? RUN_SPEED : -RUN_SPEED, RUN_SPEED, 0);
    if (s->inputs.jump.pressed) {
        s->setSpeedY(JUMP_FORCE);
        s->noMovementControlTimer = 22;
        s->setState(PlayerState::SPINJUMP);
    }
    if (s->inputs.morph.pressed && s->hasMorph)
        s->setState(PlayerState::MORPH);
    if (!s->col.isGrounded())
        s->setState(PlayerState::SLIDE_AIR_BOOST);
    if (!s->col.isHardSlopeBelow() && s->state_timer > 50)
        s->setState(PlayerState::IDLE);

    s->allowMovement(true);
    s->allowAim(AimDir_ALL & (~AimDir_DOWN));
    s->allowShoot();
}

void playerState_SlideAirBoost(Samus* s)
{
    s->applyAccelX(0, RUN_SPEED, (RUN_SPEED - AIR_SPEED_X) / 10);
    s->applyAccelY(GRAVITY, AIR_SPEED_Y, AIR_FRICTION_Y);
    if (s->col.isGrounded())
        s->setState(PlayerState::IDLE);
    if (s->state_timer > 10)
        s->setState(PlayerState::AIRNORMAL);
    if (s->inputs.jump.pressed) {
        s->setSpeedY(JUMP_FORCE);
        s->setState(PlayerState::SPINJUMP);
    }

    s->allowMovement();
    s->allowAim();
    s->allowShoot();
}

void check_bombjump(Samus* s) // TODO: Better calculation of the trajectory
{
    for (int i = 0; i < MAX_DAMAGE_SOURCE; i++) {
        auto& ds = s->getDamageSource(i);
        if (!ds.active() || ds.type != DamageSource::Type::BOMB)
            continue;
        if (ds.intersectRectangle(s->col.topleft(), s->col.bottomright())) {
            float xx = s->col.pos.x - ds.pos.x;
            xx /= 5.f;
            float yy = MORPH_JUMP_FORCE - xx / 5;
            s->setSpeedX(xx);
            s->setSpeedY(yy);
            s->setState(PlayerState::MORPH_AIR);
            return;
        }
    }
}

void playerState_Morph(Samus* s)
{
    s->applyAccelX(MORPH_ACCEL * (s->inputs.dirRi - s->inputs.dirLe), MORPH_SPEED, MORPH_FRICTION);

    if (s->inputs.jump.pressed) {
        s->setSpeedY(MORPH_JUMP_FORCE);
        s->setState(PlayerState::MORPH_AIR);
        return;
    }
    if (s->inputs.dirUp)
        s->setState(PlayerState::CROUCHING);
    if (!s->col.isGrounded())
        s->setState(PlayerState::MORPH_AIR);
    if (s->inputs.morph.pressed)
        s->setState(PlayerState::IDLE);

    s->allowMovement(true);
    s->allowPlaceBomb();
    check_bombjump(s);
}

void playerState_MorphAir(Samus* s)
{
    s->applyAccelX(MORPH_ACCEL * (s->inputs.dirRi - s->inputs.dirLe), MORPH_SPEED, MORPH_FRICTION);
    if (s->state_timer > 1)
        s->applyAccelY(GRAVITY, AIR_SPEED_Y, AIR_FRICTION_Y);

    if (s->col.isGrounded() && s->state_timer > 1)
        s->setState(PlayerState::MORPH);
    if (s->inputs.dirUp.pressed || s->inputs.morph.pressed)
        s->setState(PlayerState::IDLE);

    s->allowMovement();
    s->allowPlaceBomb();
    check_bombjump(s); // CAREFUL : WATER
}

void Samus::allowMovement(bool grounded)
{
    col.Update(spd);
    if (grounded) {
        glm::vec2 tempSpd = { 0, -1 };
        int i = 0;
        while (!col.isGrounded() && i++ < 16)
            col.Update(tempSpd);
        tempSpd = { 0, 16 };
        if (!col.isGrounded())
            col.Update(tempSpd);
    }
    // facing
    if (spd.x < 0)
        faceR = false;
    else if (spd.x > 0)
        faceR = true;
}

void Samus::allowAim(AimDir allowedAim, AimDir defaultAim)
{
    aim = getAim(inputs.dirLe, inputs.dirRi, inputs.dirUp, inputs.dirDo, inputs.dirAu, inputs.dirAd);
    if (!(aim & allowedAim))
        aim = defaultAim;
}

void Samus::allowCharge()
{
    if (!hasCharge)
        return;
    if (inputs.shoot.down)
        chargeCtr = fmin(chargeCtr + 1, 100);
}

void Samus::allowShoot()
{
    // Check for Shooting inputs
    if (Inputs::Keyboard().Down(NSK_q))
        chargeCtr2 = fmin(chargeCtr2 + 1, 100);
    else
        chargeCtr2 = 0;

    allowCharge();
    if (inputs.shoot.pressed)
        Shoot(chargeCtr2 >= 100, chargeCtr2 > 0);
    if (inputs.shoot.released) {
        if (chargeCtr > 30)
            Shoot(chargeCtr >= 100, false);
        chargeCtr = 0;
    }
}

void Samus::allowPlaceBomb()
{
    if (!hasBombs)
        return;
    bool canPBomb = hasPBombs && pbombCount > 0 && !bomb_pool[6].isActive();
    if ((hasCBombs || hasPBombs) && Inputs::Keyboard().Down(NSK_q))
        chargeCtr2 = fmin(chargeCtr2 + 1, canPBomb ? 100 : 1);
    else
        chargeCtr2 = 0;
    uint8_t typ = chargeCtr2 > 60 ? 4 : (!bomb_pool[5].isActive() && chargeCtr2) > 0 ? 2
                                                                                     : 0;
    if (!inputs.shoot.pressed)
        return;
    if (typ == 4) {
        pbombCount--;
        bomb_pool[6].Place(col.center(), typ);
        return;
    }
    if (typ == 2) {
        bomb_pool[5].Place(col.center(), typ);
        return;
    }
    int i;
    for (i = 0; i < 5; i++)
        if (!bomb_pool[i].isActive())
            break;
    if (i == 5)
        return;
    bomb_pool[i].Place(col.center(), typ);
}

void PlayerInputs::InputButton::process(int keyCode)
{
    pressed = Inputs::Keyboard().Pressed(keyCode);
    released = Inputs::Keyboard().Released(keyCode);
    down = Inputs::Keyboard().Down(keyCode);
}

void PlayerInputs::process()
{
    dirUp.process(NSK_up);
    dirDo.process(NSK_down);
    dirLe.process(NSK_left);
    dirRi.process(NSK_right);
    dirAu.process(NSK_w);
    dirAd.process(NSK_e);
    jump.process(NSK_space);
    shoot.process(NSK_s);
    morph.process(NSK_lctrl);
}

void Samus::Update()
{
    if (Area::isTransitionning())
        return;

    inputs.process();

    // Update all bullets and missiles in the pool
    for (int i = 0; i < MAX_BULLETS; i++)
        bullet_pool[i].Update();
    for (int i = 0; i < MAX_MISSILE; i++)
        missile_pool[i].Update();
    for (int i = 0; i < 7; i++)
        bomb_pool[i].Update();
    for (int i = 0; i < MAX_DAMAGE_SOURCE; i++)
        if (damage_sources[i].active() && !damage_sources[i].ttl--)
            damage_sources[i].flags = 0;

    if (Inputs::Keyboard().Pressed(NSK_i))
        col.setPos({ -160, -200 });

    // timers
    state_timer++;
    if (noMovementControlTimer)
        noMovementControlTimer--;
    if (missileCooldown)
        missileCooldown--;

    switch (state) {
    case PlayerState::FREE:
        playerState_Free(this);
        break;
    case PlayerState::IDLE:
        playerState_Idle(this);
        break;
    case PlayerState::CROUCHING:
        playerState_Crouching(this);
        break;
    case PlayerState::WALKING:
        playerState_Walk(this);
        break;
    case PlayerState::AIRNORMAL:
        playerState_Airnormal(this);
        break;
    case PlayerState::SPINJUMP:
        playerState_Spinjump(this);
        break;
    case PlayerState::SLIDING:
        playerState_Sliding(this);
        break;
    case PlayerState::SLIDE_AIR_BOOST:
        playerState_SlideAirBoost(this);
        break;
    case PlayerState::MORPH:
        playerState_Morph(this);
        break;
    case PlayerState::MORPH_AIR:
        playerState_MorphAir(this);
        break;
    }

    // TODO : This is debug
    hasSpazer = Inputs::Keyboard().Down(NSK_a);

    // TODO : get shoot point correctly
    shootPoint = (faceR ? col.right() : col.left()) - col.bottom();

    // Update all anims
    anmPositions[3] = anmPositions[4] = shootPoint;
    UpdateAnims();

    Camera::GameCamera()->SetPos(col.center().x, col.center().y);
}

void draw_tile(int x, int y, NSEngine::Color c)
{
    int xx = x / 24;
    int yy = y / 24;
    if (xx < 0)
        xx -= 1;
    if (yy < 0)
        yy -= 1;
    NSEngine::draw_rectangle_color(xx * 24, yy * 24, (xx + 1) * 24, (yy + 1) * 24, c, c, c, c);
}

// TODO : only for debug drawing, use anims otherwise
#include <Engine.hpp>
void Samus::Draw()
{
    // debug : draw adjacent tiles
    // draw_tile(col.topleft().x, col.topleft().y, {0,0,255,128});
    // draw_tile(col.topright().x, col.topright().y, {0,255,0,128});
    // draw_tile(col.bottomright().x, col.bottomright().y, {255,0,0,128});
    // draw_tile(col.bottomleft().x, col.bottomleft().y, {200,0,200,128});

    NSEngine::draw_quad_2d(col.topleft(), col.topright(), col.bottomright(), col.bottomleft());
    glm::vec2 p = col.bottom() + shootPoint;
    if (chargeCtr > 30)
        NSEngine::draw_circle(p.x, p.y, (chargeCtr - 30) / 14.f);

    if (!NSEngine::getInstance()->flags().flags.debugInfo)
        return;

    NSEngine::draw_set_layer(2);
    for (int i = 0; i < MAX_DAMAGE_SOURCE; i++)
        if (damage_sources[i].active())
            damage_sources[i].Draw();

    std::string stateNames[] = {
        "Free",
        "Idle",
        "Crouching",
        "Walking",
        "Airnormal",
        "Spinjump",
        "Sliding",
        "Slide_air_boost",
        "Morph",
        "Morph_air",
    };

    auto vec2str = [](glm::vec2 v) -> std::string { return std::to_string(v.x) + ", " + std::to_string(v.y); };
    auto veci2str = [](glm::vec2 v) -> std::string { return std::to_string((int)v.x) + ", " + std::to_string((int)v.y); };

    char bufferForSprintf[200];
    NSEngine::draw_set_layer(NSEngine::engineData::debugLayer);
    font->set_halign(SpriteFont::align::RIGHT);
    font->set_valign(SpriteFont::align::TOP);
    std::string timer_str = state_timer < 10000 ? std::to_string(state_timer) : "+9999";
    static const glm::vec2 font_s = { 144, 144 };
    font->renderText(timer_str, glm::vec2(-278, 150), font_s);
    font->set_halign(SpriteFont::align::LEFT);
    std::string state_str = stateNames[(int)state];
    font->renderText(state_str, glm::vec2(-270, 150), font_s);
    std::string pos_str = "p: " + veci2str(col.pos) + " r: " + veci2str(Area::GetRoom()->getRoomPos(col.pos));
    std::string spd_str = "s: " + vec2str(spd);
    font->renderText(pos_str, glm::vec2(-130, 160), font_s);
    font->renderText(spd_str, glm::vec2(-270, 160), font_s);
    std::string col_flags_str = "col: " + std::to_string(col.isGrounded()) + std::to_string(col.isClimbPos()) + std::to_string(col.isHardSlopeL()) + std::to_string(col.isHardSlopeR())
        + std::to_string(col.isSlopeBelow()) + std::to_string(col.isWallLeft()) + std::to_string(col.isWallRight());
    font->renderText(col_flags_str, glm::vec2(-270, 140), font_s);
    sprintf(bufferForSprintf, "%2.1f fps", NSEngine::getInstance()->fps());
    font->set_halign(SpriteFont::align::RIGHT);
    font->renderText(bufferForSprintf, glm::vec2(270, 160), font_s);
}

#define SHOOT_FIRE(p, d, s, t)                                     \
    i = 0;                                                         \
    while (bullet_pool[curBullet].isActive() && i++ < MAX_BULLETS) \
        curBullet = (curBullet + 1) % MAX_BULLETS;                 \
    bullet_pool[curBullet].Launch(p, d, s, t)
#define SPAZER_OFFSET(d) glm::vec2(-16 * sin(d), 16 * cos(d))
#define SHOOT_COLOR(t) (t < 2 ? glm::vec3(255, 240, 0) : (t < 4 ? glm::vec3(32, 240, 16) : glm::vec3(240, 0, 255)))
void Samus::Shoot(bool charge, bool missile)
{
    // Get the shoot variables
    float d = aim.getAngle();
    if (!faceR)
        d = PI - d;
    int t = charge + (hasWave ? 4 : (hasPlasma ? 2 : 0));
    glm::vec2 p = col.bottom() + shootPoint;

    if (missile) {
        if (missileCooldown > 0)
            return;
        int i = 0;
        while (missile_pool[curMissile].isActive() && i++ < MAX_MISSILE)
            curMissile = (curMissile + 1) % MAX_MISSILE;
        missile_pool[curMissile].Launch(p, d, 0);
        missileCooldown = 20;
        return;
    }

    // TODO : get the real value
    static const float BULLET_SPEED = 15;

    // touhou 9.5
    int i;
    SHOOT_FIRE(p, d, BULLET_SPEED, t);
    if (hasSpazer) {
        SHOOT_FIRE(p + SPAZER_OFFSET(d), d, BULLET_SPEED, t);
        SHOOT_FIRE(p - SPAZER_OFFSET(d), d, BULLET_SPEED, t);
    }

    // Shooting animation
    glm::vec3 col = SHOOT_COLOR(t);
    int a = AnmManager::newAnim(charge ? PlayerBeamFireC : PlayerBeamFireN);
    SetAnim(3, a, shootPoint, d, 1);
    AnmManager::anim(a)->set_color(col.r, col.g, col.b);
    if (hasSpazer) {
        a = AnmManager::newAnim(PlayerBeamFireS);
        SetAnim(4, a, shootPoint, d, 1);
        AnmManager::anim(a)->set_color(col.r, col.g, col.b);
    }
}

void Samus::SetAnim(int id, int anm)
{
    if (id < 0 || id >= MAX_ANIM)
        return;
    if (anims[id] != -1)
        AnmManager::deleteAnim(anims[id]);
    anims[id] = anm;
}

void Samus::SetAnim(int id, int anm, glm::vec2 relPos, float rot, float scale)
{
    if (id < 0 || id >= MAX_ANIM)
        return;
    if (anims[id] != -1)
        AnmManager::deleteAnim(anims[id]);
    anims[id] = anm;
    anmPositions[id] = relPos;
    anmRotations[id] = rot;
    anmScales[id] = scale;
}

void Samus::UpdateAnims()
{
    for (int i = 0; i < MAX_ANIM; i++)
        if (anims[i] != -1) {
            if (!AnmManager::IsActive(anims[i])) {
                anims[i] = -1;
                continue;
            }
            AnmManager::UpdateChild(anims[i], pos().x + anmPositions[i].x, pos().y + anmPositions[i].y,
                0, 0, 0, anmRotations[i], anmScales[i], anmScales[i], 1);
        }
}

int getAim(bool l, bool r, bool u, bool d, bool au, bool ad)
{
    if (au && ad)
        return AimDir_UP;
    if (au)
        return AimDir_FORWARD_UP;
    if (ad)
        return AimDir_FORWARD_DOWN;
    if (u) {
        if (l || r)
            return AimDir_FORWARD_UP;
        return AimDir_UP;
    }
    if (d) {
        if (l || r)
            return AimDir_FORWARD_DOWN;
        return AimDir_DOWN;
    }
    return AimDir_FORWARD;
}
