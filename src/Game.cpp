// Copyright 2023 Clément Chambard
#include "Game.h"
#include "AnmEffects.h"
#include "AnmGui.h"
#include "AnmObjects.h"
#include "AnmPlayer.h"
#include "Area.h"
#include "CollisionMap.h"
#include "Controller.h"
#include "Layers.h"
#include "PlayerRenderer.h"

const int windowHeight = 264;
const int windowWidth = 469;

// constructor + destructor
Game::Game() : IEngine(windowWidth, windowHeight, "Super Metroid Dread") {}

Area *a;
Samus *s;
// init all game objects here
void Game::on_create() {
  window().addDisplayMode({2 * windowWidth, 2 * windowHeight, 0});
  window().addDisplayMode({3 * windowWidth, 3 * windowHeight, 0});
  window().setDisplayMode(2);

  setMaxFps(60);
  NSEngine::createCamera(NS_CAM_3D, windowWidth, windowHeight);
  NSEngine::activeCamera3D()->setDebugSpeed(1.4f, 0.006f);
  InitLayers();
  AnmManager::Init();
  AnmGui::Init();
  AnmPlayer::Init();
  AnmEffects::Init();
  AnmObjects::Init();
  ControllersManager::init();
  a = new Area("");
  s = new Samus();
  a->Load();
}

void Game::on_update() {
  Area::Update();
  s->Update();
  AnmManager::Update(1);
}

void Game::on_render() {
  NSEngine::draw_set_layer(0);
  Area::Draw();
  NSEngine::draw_set_layer(0);
  s->Draw();
  AnmManager::Draw();
}

void Game::on_destroy() {
  delete a;
  delete s;
  ControllersManager::cleanup();
  Area::Cleanup();
}
