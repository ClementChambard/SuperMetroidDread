// Copyright 2023 Clément Chambard
#include "Area.h"
#include "Layers.h"
#include <NSEngine.h>
#include <fstream>

std::vector<Room *> Area::loadedRooms;
unsigned int Area::currentRoomIn = 1;
unsigned int Area::nextRoom = -1;
int Area::transitionCtr = 0;

void Area::Load() {
  Cleanup();
  LoadFromFile("artaria.area");
  currentRoomIn = 0;
  loadedRooms[currentRoomIn]->Init();
}

void Area::LoadFromFile(const std::string &file) {
  std::ifstream area_file("assets/maps/" + file);
  std::string map_file;
  while (!area_file.eof()) {
    area_file >> map_file;
    loadedRooms.push_back(new Room());
    loadedRooms.back()->LoadFromJson("assets/maps/" + map_file);
  }
}

void Area::Cleanup() {
  for (auto i : loadedRooms)
    delete i;
  loadedRooms.clear();
}

void Area::Update() {
  loadedRooms[currentRoomIn]->Update();
  if (transitionCtr > 0)
    transitionCtr++;
  if (transitionCtr == TRANSITION_WAIT) {
    currentRoomIn = nextRoom;
    loadedRooms[currentRoomIn]->Init();
  }
  if (transitionCtr >= 2 * TRANSITION_WAIT)
    transitionCtr = 0;
}

void Area::Draw() {
  loadedRooms[currentRoomIn]->Draw();
  if (transitionCtr == 0)
    return;
  NSEngine::draw_set_layer(LAYER_GUI_BELOW);
  float a = static_cast<float>(transitionCtr < TRANSITION_WAIT
                                   ? transitionCtr
                                   : (2 * TRANSITION_WAIT - transitionCtr)) /
            static_cast<float>(TRANSITION_WAIT) * 270.f;
  NSEngine::Color c = {0, 0, 0, (uint8_t)fmin(255, a)};
  NSEngine::draw_rectangle_color(-500, 500, 500, -500, c, c, c, c);
}

void Area::RoomTransition(unsigned int room, uint8_t transitionFlags) {
  transitionCtr = 1;
  nextRoom = room;
}
