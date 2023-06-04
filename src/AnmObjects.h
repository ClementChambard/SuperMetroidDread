// Copyright 2023 Clément Chambard
#ifndef ANMOBJECTS_H_
#define ANMOBJECTS_H_

#include "AnmManagerOld/AnmManager.h"

class AnmObjects {
public:
  static void Init();
};

extern uint16_t anmobject_block(int type, int x, int y, int interrupt);

#endif // ANMOBJECTS_H_
