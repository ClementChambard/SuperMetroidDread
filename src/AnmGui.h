// Copyright 2023 Clément Chambard
#ifndef ANMGUI
#define ANMGUI

#include "AnmManagerOld/AnmManager.h"
#include "Controller.h"

class AnmGui {
public:
  static void Init();
};

// void anmguiController(AnimScript* anm);
// void anmguiControllerButtonBig(AnimScript* anm);
// void anmguiControllerButtonSmall(AnimScript* anm);
// void anmguiControllerLT(AnimScript* anm);
// void anmguiControllerLB(AnimScript* anm);
// void anmguiControllerRT(AnimScript* anm);
// void anmguiControllerRB(AnimScript* anm);
// void anmguiControllerHat(AnimScript* anm);

void gui_draw_controller_widget(Controller *contr, float x, float y,
                                float s = 2, NSEngine::Color col = c_white);

#endif
