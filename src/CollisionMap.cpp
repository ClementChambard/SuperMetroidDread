// Copyright 2023 Clément Chambard
#include "CollisionMap.h"
#include <NSEngine.h>
#include <math.h>

bool CollisionMap::firstInit = false;
int CollisionMap::colsprite0;

std::vector<int> getTiles(const std::vector<uint8_t> &v) {
  std::vector<int> n;
  int l = v.size() / 6 + 1;
  for (int i = 0; i < l; i++) {
    int t = 0;
    for (int ii = 0; ii < 6; ii++) {
      if (i * 6 + ii >= v.size())
        break;
      uint8_t tt = v[i * 6 + ii];
      t |= (tt << (5 * ii));
    }
    n.push_back(t);
  }
  return n;
}

void CollisionMap::fromJsonArray(Json::Value const &val) {
  if (!firstInit)
    FirstInit();
  width = val["width"].asInt();
  height = val["height"].asInt();
  std::vector<uint8_t> v{};
  for (int i = 0; i < width * height; i++)
    v.push_back(val["collision"][i].asInt());
  tiles = v; // getTiles(v);
}

#include "AnmManagerOld/GameSprites.h"
void CollisionMap::FirstInit() {
  int texID = NSEngine::TextureManager::RegisterTexture(
      "assets/textures/Collisions.png");
  colsprite0 = GS_ AddSprites(texID, 0, 0, 16, 16, 24, 24);
  firstInit = true;
}

bool CollisionMap::collideWith(uint8_t tile, float xx, float yy) {
  switch (tile) {
  case 0:
    return true;
  case 1:
    return yy < xx;
  case 2:
    return yy < tilesize - xx;
  case 3:
    return yy < (xx + 0 * tilesize) / 2.f;
  case 4:
    return yy < (xx + 1 * tilesize) / 2.f;
  case 5:
    return yy < (-xx + 2 * tilesize) / 2.f;
  case 6:
    return yy < (-xx + 1 * tilesize) / 2.f;
  case 8:
    return tilesize - yy < (xx + 0 * tilesize) / 3.f;
  case 9:
    return tilesize - yy < (xx + 1 * tilesize) / 3.f;
  case 10:
    return tilesize - yy < (xx + 2 * tilesize) / 3.f;
  case 11:
    return tilesize - yy < (-xx + 3 * tilesize) / 3.f;
  case 12:
    return tilesize - yy < (-xx + 2 * tilesize) / 3.f;
  case 13:
    return tilesize - yy < (-xx + 1 * tilesize) / 3.f;
  case 17:
    if (xx >= tilesize - 1)
      return true; // TODO(Me): small correction : should not be needed
    return tilesize - yy < xx;
  case 18:
    return tilesize - yy < tilesize - xx;
  case 19:
    return tilesize - yy < (xx + 0 * tilesize) / 2.f;
  case 20:
    return tilesize - yy < (xx + 1 * tilesize) / 2.f;
  case 21:
    return tilesize - yy < (-xx + 2 * tilesize) / 2.f;
  case 22:
    return tilesize - yy < (-xx + 1 * tilesize) / 2.f;
  case 23:
    return tilesize - yy < (xx + 0 * tilesize) / 4.f;
  case 24:
    return tilesize - yy < (xx + 1 * tilesize) / 4.f;
  case 25:
    return tilesize - yy < (xx + 2 * tilesize) / 4.f;
  case 26:
    return tilesize - yy < (xx + 3 * tilesize) / 4.f;
  case 27:
    return tilesize - yy < (-xx + 4 * tilesize) / 4.f;
  case 28:
    return tilesize - yy < (-xx + 3 * tilesize) / 4.f;
  case 29:
    return tilesize - yy < (-xx + 2 * tilesize) / 4.f;
  case 30:
    return tilesize - yy < (-xx + 1 * tilesize) / 4.f;
  case 32:
    return tilesize - yy < 2 * xx;
  case 33:
    return tilesize - yy < -2 * xx + 2 * tilesize;
  case 34:
    return tilesize - yy < 1.5f * xx;
  case 35:
    return tilesize - yy < 1.5f * (xx + tilesize) - tilesize;
  case 36:
    return tilesize - yy < 1.5f * -xx + 2 * tilesize;
  case 37:
    return tilesize - yy < 1.5f * -(xx + tilesize) + 3 * tilesize;
  case 38:
    return tilesize - yy < (xx + 0 * tilesize) / 5.f;
  case 39:
    return tilesize - yy < (xx + 1 * tilesize) / 5.f;
  case 40:
    return tilesize - yy < (xx + 2 * tilesize) / 5.f;
  case 41:
    return tilesize - yy < (xx + 3 * tilesize) / 5.f;
  case 42:
    return tilesize - yy < (xx + 4 * tilesize) / 5.f;
  case 43:
    return tilesize - yy < (-xx + 5 * tilesize) / 5.f;
  case 44:
    return tilesize - yy < (-xx + 4 * tilesize) / 5.f;
  case 45:
    return tilesize - yy < (-xx + 3 * tilesize) / 5.f;
  case 46:
    return tilesize - yy < (-xx + 2 * tilesize) / 5.f;
  case 47:
    return tilesize - yy < (-xx + 1 * tilesize) / 5.f;
  case 48:
    if (xx >= tilesize - 1)
      return true; // TODO(Me): small correction : should not be needed
    return tilesize - yy < 2 * xx - tilesize;
  case 49:
    return tilesize - yy < -2 * xx + tilesize;
  case 50:
    return tilesize - yy < 1.5f * xx - tilesize;
  case 51:
    return tilesize - yy < 1.5f * (xx + tilesize) - 2 * tilesize;
  case 52:
    return tilesize - yy < 1.5f * -xx + tilesize;
  case 53:
    return tilesize - yy < 1.5f * -(xx + tilesize) + 2 * tilesize;
  case 64:
    return tilesize - yy < 3 * xx;
  case 65:
    return tilesize - yy < -3 * xx + 3 * tilesize;
  case 66:
    return tilesize - yy < 2 / 3.f * xx;
  case 67:
    return tilesize - yy < 2 / 3.f * (xx + tilesize);
  case 68:
    return tilesize - yy < -2 / 3.f * (xx - 2 * tilesize);
  case 69:
    return tilesize - yy < -2 / 3.f * (xx - tilesize);
  case 80:
    return tilesize - yy < 3 * xx - tilesize;
  case 81:
    return tilesize - yy < -3 * xx + 2 * tilesize;
  case 82:
    return tilesize - yy < 2 / 3.f * (xx - 2 * tilesize) + tilesize;
  case 83:
    return tilesize - yy < 2 / 3.f * (xx - tilesize) + tilesize;
  case 84:
    return tilesize - yy < -2 / 3.f * xx + tilesize;
  case 85:
    return tilesize - yy < -2 / 3.f * (xx + tilesize) + tilesize;
  case 96:
    return tilesize - yy < 3 * xx - 2 * tilesize;
    if (xx >= tilesize - 1)
      return true; // TODO(Me): small correction : should not be needed
  case 97:
    return tilesize - yy < -3 * xx + tilesize;
  }
  return false;
}

void CollisionMap::Alter(CollisionAlteration *a) {
  alterations.push_back(a);
  // std::cout << "alter\n";
}

void CollisionMap::UnAlter(CollisionAlteration *a) {
  alterations.erase(remove(alterations.begin(), alterations.end(), a),
                    alterations.end());
  // std::cout << "unalter\n";
}

bool CollisionMap::collideAt(float x, float y) {
  uint8_t t;
  return collideAt(x, y, &t);
}
bool CollisionMap::collideAt(float x, float y, uint8_t *t) {
  float xx, yy;
  *t = getTileAt(x, y, &xx, &yy);
  yy = tilesize - yy;
  return collideWith(*t, xx, yy);
}

uint8_t CollisionMap::getTileAt(float x, float y) {
  float xx, yy;
  return getTileAt(x, y, &xx, &yy);
}
uint8_t CollisionMap::getTileAt(float x, float y, float *xx, float *yy) {
  *xx = abs(static_cast<int>(x) % static_cast<int>(tilesize));
  *yy = abs(static_cast<int>(y) % static_cast<int>(tilesize));
  if (x < 0 || y < 0 || x > width * tilesize || y > height * tilesize)
    return 16;
  int row = floor(y / tilesize);
  int column = floor(x / tilesize);

  for (auto a : alterations) {
    if (row < a->starty || row >= a->starty + a->h || column < a->startx ||
        column >= a->startx + a->w)
      continue;
    // get tile in alteration
    int newrow = row - a->starty;
    int newcolumn = column - a->startx;
    return a->tiles[a->w * newrow + newcolumn];
  }
  int thetile = width * row + column;
  return tiles[thetile]; //(tiles[thetile / tileperblock] >> ((thetile %
                         // tileperblock) * 5) ) & 0b11111;
}
uint8_t CollisionMap::getTileAtNoAlter(float x, float y) {
  if (x < 0 || y < 0 || x > width * tilesize || y > height * tilesize)
    return 16;
  int row = floor(y / tilesize);
  int column = floor(x / tilesize);
  int thetile = width * row + column;
  return tiles[thetile]; //(tiles[thetile / tileperblock] >> ((thetile %
                         // tileperblock) * 5) ) & 0b11111;
}
#include "Area.h"

uint8_t CollisionMap::getTileAtGlobalPos(float x, float y) {
  auto p = Area::GetRoom()->getRoomPos({x, -y});
  return getTileAt(p.x, p.y);
}

void CollisionMap::draw(glm::vec2 worldpos) {
  NSEngine::draw_set_layer(0);
  float x = 0;
  float y = 0;
  for (int i = 0; i < tiles.size(); i++) {
    if (i % width == 0 && i != 0) {
      x = 0;
      y += tilesize;
    }
    if (y >= tilesize * height)
      break;
    uint8_t t = getTileAt(x, y);
    if (t != 16)
      NSEngine::draw_game_sprite(colsprite0 + t,
                                 worldpos.x + x + 0.5 * tilesize,
                                 worldpos.y - y - 0.5 * tilesize, 1, 1, 0,
                                 c_white, c_white, c_white, c_white);
    x += tilesize;
  }
}
