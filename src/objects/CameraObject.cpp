#include "CameraObject.h"
#include "Engine.h"
#include "utils.h"

#include <math.h>

using namespace engine;

CameraObject *CameraObject::single_camera = nullptr;

CameraObject::CameraObject(void) {
  if (single_camera == nullptr) {
    single_camera = this;
    return;
  }

  CameraObject *old_camera = single_camera;
  single_camera = this;
  delete old_camera;
}

CameraObject::~CameraObject(void) {
  if (single_camera == this) {
    single_camera = nullptr;
  }
}

CameraObject *CameraObject::getCamera(void) { return single_camera; }

void CameraObject::frame(void) {
  auto keys = Engine::getEngine()->getPressedKeys();

  if (keys.find(GLFW_KEY_I) != keys.end()) {
    position[1] += 0.05;
  }
  if (keys.find(GLFW_KEY_U) != keys.end()) {
    position[1] -= 0.05;
  }
  if (keys.find(GLFW_KEY_Y) != keys.end()) {
    position[0] -= 0.05;
  }
  if (keys.find(GLFW_KEY_O) != keys.end()) {
    position[0] += 0.05;
  }

  if (keys.find(GLFW_KEY_R) != keys.end()) {
    position[2] += 0.05;
  }
  if (keys.find(GLFW_KEY_T) != keys.end()) {
    position[2] -= 0.05;
  }

  for (int i = 0; i < 3; i++) {
    if (position[i] > 10) {
      position[i] = 10;
    }
    if (position[i] < -10) {
      position[i] = -10;
    }
  }
}

void CameraObject::mouseCallback(GLFWwindow *w, double x, double y) {
  auto *c = getCamera();
  if (c == nullptr) {
    return;
  }

  const float sensitivity = 0.1 * M_PI_2 / 180;

  c->view_angle[0] += -x * sensitivity;
  c->view_angle[1] += y * sensitivity;

  if (c->view_angle[1] > M_PI_2 - 0.1) {
    c->view_angle[1] = M_PI_2 - 0.1;
  } else if (c->view_angle[1] < -M_PI_2 + 0.1) {
    c->view_angle[1] = -M_PI_2 + 0.1;
  }

  if (c->view_angle[0] < 0) {
    c->view_angle[0] = 2 * M_PI;
  } else if (c->view_angle[0] > 2 * M_PI) {
    c->view_angle[0] = 0;
  }

  glfwSetCursorPos(w, 0, 0);
}
