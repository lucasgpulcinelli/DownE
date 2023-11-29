#include "Engine.h"
#include "utils.h"

#include "objects/Object3DWithMovement.h"
#include "objects/LightObject.h"

extern "C" {
#include <GLFW/glfw3.h>
}

void firstScreen(void) {
  debug("adding initial elements");
  engine::Engine *e = engine::Engine::getEngine();

  float light_position[3] = {0, 0, 0};
  float light_color[3] = {1, 1, 1};

  e->addObject(new engine::LightObject(light_position, light_color));
  e->addObject(new engine::Object3DWithMovement("cube", GLFW_KEY_1));
}

int main() {
  auto e = engine::Engine();

  firstScreen();
  e.run();
}
