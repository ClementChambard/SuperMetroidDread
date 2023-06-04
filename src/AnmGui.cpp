// Copyright 2023 Clément Chambard
#include "AnmGui.h"
#include <NSEngine.h>

int guiContr1a;
int guiContr1b;
int guiContr2a;
int guiContr2b;
int guiContr3a;
int guiContr3b;
int guiContr4a;
int guiContr4b;
int guiContr5a;
int guiContr5b;
int guiContrFrame;

void AnmGui::Init() {
  int t = NSEngine::TextureManager::RegisterTexture(
      "assets/textures/controller.png");
  guiContr1a = GS_ AddSprite(t, 0, 0, 16, 16);
  guiContr1b = GS_ AddSprite(t, 0, 16, 16, 16);
  guiContr2a = GS_ AddSprite(t, 16, 0, 16, 16);
  guiContr2b = GS_ AddSprite(t, 16, 16, 16, 16);
  guiContr3a = GS_ AddSprite(t, 32, 0, 16, 16);
  guiContr3b = GS_ AddSprite(t, 32, 16, 16, 16);
  guiContr4a = GS_ AddSprite(t, 48, 0, 16, 16);
  guiContr4b = GS_ AddSprite(t, 48, 16, 16, 16);
  guiContr5a = GS_ AddSprite(t, 64, 0, 16, 16);
  guiContr5b = GS_ AddSprite(t, 64, 16, 16, 16);
  guiContrFrame = GS_ AddSprite(t, 64, 48, 144, 96);
}

#define fourtimes(a) a, a, a, a

void gui_draw_controller_widget(Controller *contr, float x, float y, float s,
                                NSEngine::Color col) {
  NSEngine::draw_set_layer(NSEngine::engineData::debugLayer);
  NSEngine::draw_game_sprite(guiContrFrame, x, y, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::L, 1, 0) ? guiContr1b
                                                         : guiContr1a,
                             x - s * 32, y + s * 40, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::R, 1, 0) ? guiContr1b
                                                         : guiContr1a,
                             x + s * 32, y + s * 40, -s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::ZL, 1, 0) ? guiContr2b
                                                          : guiContr2a,
                             x - s * 32, y + s * 40, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::ZR, 1, 0) ? guiContr2b
                                                          : guiContr2a,
                             x + s * 32, y + s * 40, -s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::Select, 1, 0) ? guiContr3b
                                                              : guiContr3a,
                             x - s * 8, y + s * 20, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::Start, 1, 0) ? guiContr3b
                                                             : guiContr3a,
                             x + s * 8, y + s * 20, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::X, 1, 0) ? guiContr4b
                                                         : guiContr4a,
                             x + s * 20, y + s * 19, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::Y, 1, 0) ? guiContr4b
                                                         : guiContr4a,
                             x + s * 27, y + s * 26, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::A, 1, 0) ? guiContr4b
                                                         : guiContr4a,
                             x + s * 27, y + s * 12, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::B, 1, 0) ? guiContr4b
                                                         : guiContr4a,
                             x + s * 34, y + s * 19, s, s, 0, fourtimes(col));
  glm::vec2 p1 =
      s * 8.f * contr->StickPos(true) + glm::vec2(x - s * 31, y + s * 19);
  glm::vec2 p2 =
      s * 8.f * contr->StickPos(false) + glm::vec2(x + s * 16, y - s * 4);
  NSEngine::draw_game_sprite(contr->Button(BTN::LS, 1, 0) ? guiContr4b
                                                          : guiContr4a,
                             p1.x, p1.y, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::RS, 1, 0) ? guiContr4b
                                                          : guiContr4a,
                             p2.x, p2.y, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(
      contr->Button(BTN::DpadU, 1, 0) ? guiContr5b : guiContr5a, x - s * 16,
      y + 2 * s, s, s, PI1_2, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::DpadL, 1, 0) ? guiContr5b
                                                             : guiContr5a,
                             x - s * 22, y - 4 * s, s, s, 0, fourtimes(col));
  NSEngine::draw_game_sprite(contr->Button(BTN::DpadR, 1, 0) ? guiContr5b
                                                             : guiContr5a,
                             x - s * 10, y - 4 * s, s, s, PI, fourtimes(col));
  NSEngine::draw_game_sprite(
      contr->Button(BTN::DpadD, 1, 0) ? guiContr5b : guiContr5a, x - s * 16,
      y - 10 * s, s, s, -PI1_2, fourtimes(col));
}
