// Copyright 2023 Clément Chambard
#ifndef CONTROLLER
#define CONTROLLER

#include <InputManager.h>
#include <glm/glm.hpp>
#include <vector>

enum BTN {
  A = 0,
  B,
  X,
  Y,
  L,
  R,
  ZL,
  ZR,
  Select,
  Start,
  LS,
  RS,
  DpadU,
  DpadD,
  DpadL,
  DpadR,
  NBOFBUTTON,
};

class Controller {
public:
  explicit Controller(int joystick);
  ~Controller();

  void Update(bool count);
  void Draw();

  bool Button(BTN input, bool hold, bool del, unsigned int buff = 6);
  glm::vec2 StickPos(bool isLeftStick);
  bool StickFlickX(bool isLeftStick, unsigned int buff = 3);
  bool StickFlickY(bool isLeftStick, unsigned int buff = 3);

private:
  struct Stick {

    Stick(bool isLeftStick, int joystick);

    void cache_values(bool count);
    void Draw(float x, float y, float size);

    int joystick = -1;
    bool isLeftStick;
    std::vector<glm::vec2> sensibilities = std::vector<glm::vec2>(2, {0, 0});
    std::vector<float> flickedX = std::vector<float>(10, 0);
    std::vector<float> flickedY = std::vector<float>(10, 0);

    // callibration
    float maxX = 1;
    float minX = 1;
    float maxY = 1;
    float minY = 1;

    static std::vector<int> LStiKbdMap;
    static std::vector<int> RStiKbdMap;
  };

  struct BtnBuff {
    unsigned char hold = 60, press = 60;
  };

  int joystick = -1;
  Stick *lstick;
  Stick *rstick;
  std::vector<BtnBuff> InputBuffer = std::vector<BtnBuff>(NBOFBUTTON);
  static std::vector<int> BtnKbdMap;
};

class ControllersManager {
public:
  static void init();
  static void refresh();
  static void cleanup();
  static void drawGui();
  static Controller *getController(int i) {
    if (i + 1 >= controllers.size())
      return controllers[0];
    else
      return controllers[i + 1];
  }

private:
  static std::vector<Controller *> controllers;
};

#endif
