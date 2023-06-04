// Copyright 2023 Clément Chambard
#include "Room.h"
#include "Area.h"
#include "Camera.h"
#include <NSEngine.h>
#include <fstream>

void Room::Init() {
  float x1 = worldPos.x;
  float y1 = worldPos.y - size.y;
  float x2 = worldPos.x + size.x;
  float y2 = worldPos.y;
  Camera::GameCamera()->SetBoundaries(x1, y1, x2, y2);
}

void Room::Update() {
  for (auto &d : doors)
    d.Update(Samus::instance);
  for (auto &b : blocs)
    b.Update();
}

void Room::LoadFromJson(std::string fileName) {
  std::ifstream file(fileName);
  Json::Reader reader;
  Json::Value val;
  reader.parse(file, val);
  file.close();

  collision.xx = worldPos.x = val["x"].asInt();
  collision.yy = worldPos.y = val["y"].asInt();

  for (auto t : val["transitions"]) {
    transitions.push_back({{t["x1"].asInt(), t["y1"].asInt()},
                           {t["x2"].asInt(), t["y2"].asInt()},
                           (unsigned char)t["dest"].asInt(),
                           (unsigned char)t["flags"].asInt()});
  }
  std::string meta = val["meta"].asString();
  if (meta != "") {
    std::ifstream metaf(meta);
    Json::Reader reader;
    Json::Value val;
    reader.parse(metaf, val);
    metaf.close();
    LoadMetaFromJson(val);
    return;
  }
  LoadMetaFromJson(val);
}

void Room::LoadMetaFromJson(Json::Value const &val) {
  size.x = val["width"].asInt() * 24;
  size.y = val["height"].asInt() * 24;
  collision.fromJsonArray(val);
  blocs.reserve(val["blocs"].size());
  for (auto b : val["blocs"]) {
    blocs.emplace_back((BlockType)b["t"].asInt(), b["x"].asInt(),
                       b["y"].asInt(), &collision);
  }
}

void Room::Draw() {
  collision.draw(worldPos);
  for (auto &b : blocs)
    b.Draw();
  // worldPos *= glm::vec2(1,-1);
  // for (auto t : transitions)
  //{
  //     glm::vec2 pos1 = worldPos + t.pos1;
  //     glm::vec2 pos2 = worldPos + t.pos2;
  //     NSEngine::draw_rectangle_color(pos1.x,-pos1.y,pos2.x,-pos2.y, c_white,
  //     c_white, c_white, c_white);
  // }
  // worldPos *= glm::vec2(1,-1);
}

glm::vec2 Room::getRoomPos(glm::vec2 worldpos) const {
  return worldpos - this->worldPos * glm::vec2(1, -1);
}

void moveForTransition(RoomTransition const &transition, Collider *c) {
  uint8_t dir = transition.transitionFlags & 0b11;
  float w = transition.pos1.x - transition.pos2.x;
  switch (dir) {
  case 0:
    c->setPos(c->bottom() + glm::vec2(-w * 2, 0));
    break;
  case 1:
    c->setPos(c->bottom() + glm::vec2(w * 2, 0));
    break;
  case 2:
    c->setPos(c->bottom() + glm::vec2(0, c->h()));
    break;
  case 3:
    c->setPos(c->bottom() + glm::vec2(0, -c->h()));
    break;
  }
}

void Room::checkRoomTransition(Collider *col) const {
  glm::vec2 c1 = (worldPos - col->topleft()) * glm::vec2(-1, 1);
  glm::vec2 c2 = (worldPos - col->bottomright()) * glm::vec2(-1, 1);

  for (auto t : transitions) {
    if (c1.x > t.pos2.x || c2.x < t.pos1.x || c2.y < t.pos1.y ||
        c1.y > t.pos2.y)
      continue;
    Area::RoomTransition(t.roomnumber, t.transitionFlags);
    moveForTransition(t, col);
    break;
  }
}
