// Copyright 2023 Clément Chambard
#include "Camera.h"
#include <Engine.hpp>
#include <NSEngine.h>

Camera *Camera::gameCamera = nullptr;

Camera *Camera::GameCamera() {
  if (!gameCamera)
    gameCamera = new Camera();
  return gameCamera;
}

void Camera::Cleanup() {
  if (gameCamera)
    delete gameCamera;
  gameCamera = nullptr;
}

Camera::Camera() {
  this->z =
      NSEngine::getInstance()->window().getWindowData().bheight / 0.828427f;
}
Camera::~Camera() {}

void Camera::SetBoundaries(float x1, float y1, float x2, float y2) {
  bx1 = x1;
  by1 = y1;
  bx2 = x2;
  by2 = y2;
}

void Camera::SetZ(float z) { this->z = z; }

void Camera::SetPos(float x, float y) {
  float w, h;
  GetViewSize(&w, &h);
  float minx = bx1 + w / 2, maxx = bx2 - w / 2;
  float miny = by1 + h / 2, maxy = by2 - h / 2;
  if (minx > maxx)
    x = (minx + maxx) / 2;
  else
    x = math::clamp(x, minx, maxx);
  if (miny > maxy)
    y = (miny + maxy) / 2;
  else
    y = math::clamp(y, miny, maxy);
  NSEngine::activeCamera3D()->setPosition(x, y, z);
}

void Camera::GetViewSize(float *w, float *h) const {
  *h = 0.828427f * this->z;
  auto d = NSEngine::getInstance()->window().getWindowData();
  *w = *h * d.bwidth / static_cast<float>(d.bheight);
}
