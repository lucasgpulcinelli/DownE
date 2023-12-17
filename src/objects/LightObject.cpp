#include "LightObject.h"

#include "Object3DWithLight.h"

#include "Engine.h"
#include "utils.h"

using namespace engine;

LightObject *LightObject::single_light = nullptr;

LightObject::LightObject(float position[3], float color[3])
    : Object3DWithLight(position, "simple3d", "light") {

  if (single_light != nullptr) {
    error("error: more than one light source is not supported");
  }

  single_light = this;

  std::copy_n(position, 3, this->position);
  std::copy_n(color, 3, this->color);

  auto w_h = Engine::getEngine()->getWindowSize();
  float ar = float(w_h.second) / w_h.first;
  scales[0] = ar;
}

LightObject::~LightObject(void) { single_light = nullptr; }

void LightObject::frame(void) {
  auto keys = Engine::getEngine()->getPressedKeys();

  if (keys.find(GLFW_KEY_K) != keys.end()) {
    position[2] += 0.05;
  }
  if (keys.find(GLFW_KEY_J) != keys.end()) {
    position[2] -= 0.05;
  }
  if (keys.find(GLFW_KEY_H) != keys.end()) {
    position[0] -= 0.05;
  }
  if (keys.find(GLFW_KEY_L) != keys.end()) {
    position[0] += 0.05;
  }

  if (keys.find(GLFW_KEY_G) != keys.end()) {
    position[1] -= 0.05;
  }
  if (keys.find(GLFW_KEY_F) != keys.end()) {
    position[1] += 0.05;
  }

  checkSkybox();
}

const float *LightObject::getPosition(void) { return single_light->position; }

const float *LightObject::getColor(void) { return single_light->color; }
