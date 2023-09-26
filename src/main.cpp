#include "Engine.h"
#include "utils.h"

#include "objects/ColorfulSpaceShip.h"
#include "objects/Cube.h"

void firstScreen(void) {
  debug("adding initial elements");
  engine::Engine::getEngine()->addObject(new objects::ColorfulSpaceship(0, 0));
  engine::Engine::getEngine()->addObject(new objects::Cube());
}

int main() {
  auto e = engine::Engine();

  firstScreen();
  e.run();
}
