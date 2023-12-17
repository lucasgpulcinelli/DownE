#include "Engine.h"
#include "utils.h"

#include "objects/LightObject.h"
#include "objects/Object3DWithLight.h"
#include "objects/Object3DWithMovement.h"
#include "objects/Skybox.h"

extern "C" {
#include <GLFW/glfw3.h>
}

void firstScreen(void) {
  debug("adding initial elements");
  engine::Engine *e = engine::Engine::getEngine();

  float light_position[3] = {-3, 3, 0};
  float light_color[3] = {1, 0, 0};
  float cube_pos[3] = {4, 4, 0};

  e->addObject(new engine::LightObject(light_position, light_color));
  e->addObject(new engine::Skybox);

  e->addObject(new engine::Object3DWithMovement(cube_pos, "cube", GLFW_KEY_1));
}

int main() {
  auto e = engine::Engine();

  firstScreen();
  e.run();
}
