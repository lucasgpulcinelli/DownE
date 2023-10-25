#include "Engine.h"
#include "utils.h"

#include "objects/Object3DWithMovement.h"

extern "C" {
#include <GLFW/glfw3.h>
}

void firstScreen(void) {
  debug("adding initial elements");
  engine::Engine::getEngine()->addObject(
      new engine::Object3DWithMovement("cube", GLFW_KEY_1));
}

int main() {
  auto e = engine::Engine();

  firstScreen();
  e.run();
}
