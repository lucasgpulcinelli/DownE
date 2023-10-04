#include "Engine.h"
#include "utils.h"

#include "objects/Cube.h"

void firstScreen(void) {
  debug("adding initial elements");
  engine::Engine::getEngine()->addObject(new engine::Cube());
}

int main() {
  auto e = engine::Engine();

  firstScreen();
  e.run();
}
