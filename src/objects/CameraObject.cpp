#include "CameraObject.h"
#include "Engine.h"
#include "Skybox.h"
#include "utils.h"

#include <cmath>
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

  if (keys.find(GLFW_KEY_W) != keys.end()) {
    position[0] -= 0.05 * std::sin(view_angle[0]);
    position[2] -= 0.05 * std::cos(view_angle[0]);
  }
  if (keys.find(GLFW_KEY_S) != keys.end()) {
    position[0] += 0.05 * std::sin(view_angle[0]);
    position[2] += 0.05 * std::cos(view_angle[0]);
  }
  if (keys.find(GLFW_KEY_A) != keys.end()) {
    position[0] -= 0.05 * std::cos(view_angle[0]);
    position[2] += 0.05 * std::sin(view_angle[0]);
  }
  if (keys.find(GLFW_KEY_D) != keys.end()) {
    position[0] += 0.05 * std::cos(view_angle[0]);
    position[2] -= 0.05 * std::sin(view_angle[0]);
  }

  if (keys.find(GLFW_KEY_SPACE) != keys.end()) {
    position[1] += 0.05;
  }
  if (keys.find(GLFW_KEY_E) != keys.end()) {
    position[1] -= 0.05;
  }

  float skyboxr = Skybox::getSkyboxRadius();

  float hp = std::hypot(position[0], position[1], position[2]);
  if (hp > skyboxr - 0.8) {
    for (int i = 0; i < 3; i++) {
      position[i] *= (skyboxr - 0.8) / hp;
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
