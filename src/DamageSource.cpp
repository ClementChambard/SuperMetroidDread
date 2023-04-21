#include "DamageSource.hpp"
#include "Samus.h"
#include <math/math.h>

void DamageSource::Activate(glm::vec2 position, float size, int dmg, Type type, uint8_t obj_id, int ttl)
{
    this->pos = position;
    this->damage = dmg;
    this->size = size;
    this->type = type;
    this->obj_id = obj_id;
    this->ttl = ttl;
    this->spd = {0,0};

    flags = 1;
}

bool DamageSource::intersectCircle(glm::vec2 pos, float size) const
{
    return (pos.x - this->pos.x) * (pos.x - this->pos.x) + (pos.y - this->pos.y) * (pos.y - this->pos.y) < (size + this->size) * (size + this->size) ||
           (pos.x + spd.x - this->pos.x) * (pos.x + spd.x - this->pos.x) + (pos.y + spd.y - this->pos.y) * (pos.y + spd.y - this->pos.y) <
           (size + this->size) * (size + this->size);
}

bool DamageSource::intersectRectangle(glm::vec2 pos1, glm::vec2 pos2) const
{
    // get closest point on rectangle
    float xx = math::clamp(pos.x, pos1.x, pos2.x);
    float yy = math::clamp(pos.y, pos1.y, pos2.y);

    // distance
    bool b1 = (pos.x - xx) * (pos.x - xx) + (pos.y - yy) * (pos.y - yy) < size * size;
    if (b1) return true;

    xx = math::clamp(pos.x + spd.x, pos1.x, pos2.x);
    yy = math::clamp(pos.y + spd.y, pos1.y, pos2.y);

    return (pos.x + spd.x - xx) * (pos.x + spd.x - xx) + (pos.y + spd.y - yy) * (pos.y + spd.y - yy) < size * size;
}

void DamageSource::hit(bool terrain [[maybe_unused]])
{
    if (isBeam()) {
        Samus::instance->bullet_pool[obj_id].Break();
    }
    if (isMissile()) {
        Samus::instance->missile_pool[obj_id].Break();
    }
}

#include <DrawFuncs.h>
void DamageSource::Draw() const
{
    NSEngine::draw_circle_color(pos.x, pos.y, size, {255,0,0,128}, {255,0,0,128});
    NSEngine::draw_circle_color(pos.x+spd.x, pos.y+spd.y, size, {255,0,0,64}, {255,0,0,64});
}
