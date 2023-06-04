// Copyright 2023 Clément Chambard
#include <NSEngine.h>

#define LAYER_MAIN_BELOW 0
#define LAYER_MAIN 1
#define LAYER_MAIN_ABOVE 2
#define LAYER_GUI_BELOW 3
#define LAYER_GUI 4
#define LAYER_GUI_ABOVE 5

inline void InitLayers() {
  NSEngine::addGameLayer();
  NSEngine::addGameLayer();
  NSEngine::addGameLayer();
  NSEngine::addGameLayer(false, true);
  NSEngine::addGameLayer(false, true);
  NSEngine::addGameLayer(false, true);
}
