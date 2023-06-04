// Copyright 2023 Clément Chambard
#define SDL_MAIN_HANDLED
#define NS_PROJECT_FOLDER(x) "~/dev/SmileDestructor/ ## x ## "
#include "Game.h"

int main(int argc, const char *argv[]) {
  Game().run();
  return 0;
}
