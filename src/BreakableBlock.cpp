#include "BreakableBlock.h"
#include "AnmObjects.h"
#include "Area.h"
#include "Samus.h"

BreakableBlock::BreakableBlock(BlockType t, int gridX, int gridY, CollisionMap* map)
{
    type = t;

    col.w = 1;
    col.h = 1;
    col.startx = gridX;
    col.starty = gridY - 1;
    col.tiles = { 0 };

    this->map = map;
    map->Alter(&col);
    anmid = anmobject_block((int)t, gridX * 24 - map->xx, gridY * 24 - map->yy, 0);
}

BreakableBlock::~BreakableBlock()
{
    map->UnAlter(&col);
    AnmManager::deleteAnim(anmid);
}

void BreakableBlock::Update()
{
    if (reformTimer > 0)
        reformTimer--;
    if (!Samus::instance->col.onTile(col.startx, col.starty))
    {
        if (reformTimer == 0) {
            reformTimer = -1;
            col.tiles[0] = 0;
            AnmManager::interrupt(anmid, 2);
        }
    }
    if (reformTimer >= 0)
        return;

    glm::vec2 pos1 = {col.startx * 24 + map->xx, -col.starty * 24 + map->yy};
    glm::vec2 pos2 = {col.startx * 24 + map->xx + 24, -col.starty * 24 + map->yy - 24};

    if (type == BlockType::CRUMBLE) {
        bool on_tile = Samus::instance->col.onTile(col.startx, col.starty - 1);
        bool b1 = ((int)(Samus::instance->col.bottom().y) % 24) < 4;
        bool b2 = ((int)(Samus::instance->col.bottom().y + Samus::instance->spd.y) % 24) < 4;
        if (on_tile && (b1 || b2)) {

            reformTimer = 10;
            col.tiles[0] = 16;
            AnmManager::interrupt(anmid, 1);
        }
    }

    auto try_break_bloc = [this, pos1, pos2] (int i) {
        if (Samus::instance->getDamageSource(i).intersectRectangle(pos1, pos2)) {
            Samus::instance->getDamageSource(i).hit();
            reformTimer = 300;
            col.tiles[0] = 16;
            AnmManager::interrupt(anmid, 1);
        }
    };
    auto try_reveal_bloc = [this, pos1, pos2] (int i) {
        // do things
    };

    using Typ = DamageSource::Type;
    for (int i = 0; i < MAX_DAMAGE_SOURCE; i++) {
        auto& ds = Samus::instance->getDamageSource(i);
        if (!ds.active()) continue;
        if (type == BlockType::BEAM) { try_break_bloc(i); return; }
        if (type == BlockType::MISSILE && ds.isMissile()) { try_break_bloc(i); return; }
        if (type == BlockType::BOMB && (ds.type == Typ::BOMB || ds.type == Typ::POWER_BOMB)) { try_break_bloc(i); return; }
        if (type == BlockType::PBOMB && ds.type == Typ::POWER_BOMB) { try_break_bloc(i); return; }
        if (type == BlockType::SCREW && ds.type == Typ::SCREW_ATTACK) { try_break_bloc(i); return; }
        if (type == BlockType::SPEEDBOOST && ds.type == Typ::SPEED_BOOST) { try_break_bloc(i); return; } // SPEED BOOSTER BLOCK SHOULD DESTROY IN GROUP
        if (ds.type == Typ::BOMB || ds.type == Typ::POWER_BOMB || ds.type == Typ::MISSILE) try_reveal_bloc(i);
    }
}

void BreakableBlock::Draw()
{
    AnmManager::UpdateChild(anmid, col.startx * 24 + map->xx + 12, -col.starty * 24 + map->yy - 12, 0, 0, 0, 0, 1, 1, true);
}
