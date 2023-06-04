// Copyright 2023 Clément Chambard
#include "Controller.h"
#include <math/math.h>

std::vector<int> Controller::BtnKbdMap = {
    NSK_lctrl,  NSK_lalt,      NSK_space, NSK_v,         NSK_w, NSK_s,
    NSK_a,      NSK_d,         NSK_q,     NSK_1,         NSK_e, NSK_3,
    NSK_return, NSK_backspace, NSK_p,     NSK_semicolon,
};

std::vector<int> Controller::Stick::LStiKbdMap = {NSK_up, NSK_down, NSK_left,
                                                  NSK_right, NSK_lshift};
std::vector<int> Controller::Stick::RStiKbdMap = {NSK_kp8, NSK_kp2, NSK_kp4,
                                                  NSK_kp6, NSK_kp5};

Controller::Controller(int joystick) {
  this->joystick = joystick;
  lstick = new Stick(true, joystick);
  rstick = new Stick(false, joystick);
}

Controller::~Controller() {
  delete lstick;
  delete rstick;
}

void Controller::Update(bool count) {
  lstick->cache_values(count);
  rstick->cache_values(count);

  if (count) {
    for (size_t i = 0; i < InputBuffer.size(); i++) {
      InputBuffer[i].hold = fmin(InputBuffer[i].hold + 1, 60);
      InputBuffer[i].press = fmin(InputBuffer[i].press + 1, 60);
    }
  }

  if (joystick == -1) {
    for (size_t i = 0; i < InputBuffer.size(); i++) {
      if (Inputs::Keyboard().Down(BtnKbdMap[i]))
        InputBuffer[i].hold = 0;
      if (Inputs::Keyboard().Pressed(BtnKbdMap[i]))
        InputBuffer[i].press = 0;
    }
  } else {
    for (size_t i = 0; i < InputBuffer.size(); i++) {
      if (Inputs::Gamepad(joystick).Down(i))
        InputBuffer[i].hold = 0;
      if (Inputs::Gamepad(joystick).Pressed(i))
        InputBuffer[i].press = 0;
    }
  }
}

bool Controller::Button(BTN input, bool hold, bool del, unsigned int buff) {
  if (hold && InputBuffer[input].hold <= buff ||
      !hold && InputBuffer[input].press <= buff) {
    if (del) {
      if (hold)
        InputBuffer[input].hold = 60;
      else
        InputBuffer[input].press = 60;
    }
    return true;
  }
  return false;
}

glm::vec2 Controller::StickPos(bool isLeftStick) {
  Stick *st = isLeftStick ? lstick : rstick;
  return st->sensibilities[0];
}

bool Controller::StickFlickX(bool isLeftStick, unsigned int buff) {
  if (buff > 10)
    buff = 10;
  Stick *st = isLeftStick ? lstick : rstick;
  float dist = 0;
  for (unsigned int i = 0; i < buff; i++)
    dist += st->flickedX[i];
  return abs(dist) > 0.5f &&
         math::sign(dist) == -math::sign(st->sensibilities[0].x);
}
bool Controller::StickFlickY(bool isLeftStick, unsigned int buff) {
  if (buff > 10)
    buff = 10;
  Stick *st = isLeftStick ? lstick : rstick;
  float dist = 0;
  for (unsigned int i = 0; i < buff; i++)
    dist += st->flickedY[i];
  return abs(dist) > 0.5f &&
         math::sign(dist) == -math::sign(st->sensibilities[0].y);
}

Controller::Stick::Stick(bool isLeftStick, int joystick) {
  this->joystick = joystick;
  this->isLeftStick = isLeftStick;
}

void Controller::Stick::cache_values(bool count) {
  float trueX = 0.f;
  float trueY = 0.f;
  if (count) {
    sensibilities[1] = sensibilities[0];
    for (size_t i = 9; i > 0; i--) {
      flickedX[i] = flickedX[i - 1];
      flickedY[i] = flickedY[i - 1];
    }
  }

  if (joystick == -1) {
    std::vector<int> k = isLeftStick ? LStiKbdMap : RStiKbdMap;
    float amt = Inputs::Keyboard().Down(k[4]) ? .5f : 1;
    trueX = amt * 127 *
            (Inputs::Keyboard().Down(k[3]) - Inputs::Keyboard().Down(k[2]));
    trueY = amt * 127 *
            (Inputs::Keyboard().Down(k[0]) - Inputs::Keyboard().Down(k[1]));
    if (trueX != 0 && trueY != 0) {
      trueX /= sqrt(2);
      trueY /= sqrt(2);
    }
  } else {
    float x = Inputs::Gamepad(joystick).AxisValue(0 + 2 * !isLeftStick);
    x /= 32768.f;
    trueX = 127 * x;
    float y = -Inputs::Gamepad(joystick).AxisValue(1 + 2 * !isLeftStick);
    y /= 32768.f;
    trueY = 127 * y;
  }

  if (abs(trueX) < 22)
    trueX = 0;
  else
    trueX = fmin(abs(trueX) / 120, 1) * math::sign(trueX);
  if (abs(trueY) < 22)
    trueY = 0;
  else
    trueY = fmin(abs(trueY) / 120, 1) * math::sign(trueY);

  sensibilities[0] = {trueX, trueY};
  flickedX[0] = sensibilities[1].x - trueX;
  flickedY[0] = sensibilities[1].y - trueY;
}

#include <Engine.hpp>
#include <NSEngine.h>

void Controller::Draw() {
  NSEngine::draw_set_layer(NSEngine::engineData::debugLayer);
  for (size_t i = 0; i < InputBuffer.size(); i++) {
    NSEngine::draw_rectangle(0 + i * 20, 0, 10 + i * 20, InputBuffer[i].hold);
    NSEngine::draw_rectangle(10 + i * 20, 0, 22 + i * 20, InputBuffer[i].press);
  }
  lstick->Draw(80, 120, 50);
  rstick->Draw(200, 120, 50);
}

void Controller::Stick::Draw(float x, float y, float r) {
  NSEngine::draw_circle(x, y, r, true);
  NSEngine::draw_circle(x + r * sensibilities[0].x, y + r * sensibilities[0].y,
                        r / 3.f, false);
}

std::vector<Controller *> ControllersManager::controllers;
void ControllersManager::init() {
  controllers.push_back(new Controller(-1));
  Inputs::InitGamepads();
  for (int i = 0; i < NSEngine::GamepadStruct::Number(); i++)
    controllers.push_back(new Controller(i));
}
void ControllersManager::refresh() {
  cleanup();
  init();
}
void ControllersManager::cleanup() {
  for (int i = 0; i < controllers.size(); i++)
    delete controllers[i];
  controllers.clear();
}

#include "AnmGui.h"
void ControllersManager::drawGui() {
  if (NSEngine::getInstance()->flags().val & 0b00000010) {
    for (int i = 0; i < controllers.size(); i++)
      gui_draw_controller_widget(controllers[i], -480 + i * 400, 360, 2,
                                 c_white);
  } else {
    for (int i = 0; i < controllers.size(); i++)
      gui_draw_controller_widget(controllers[i], -600 + i * 100, 450, 0.5f,
                                 c_white);
  }
}
