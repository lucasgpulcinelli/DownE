#include "Engine.h"
#include "utils.h"

#include "objects/ColorfulSpaceShip.h"

void firstScreen(void) {
  debug("adding initial elements");
  engine::Engine::getEngine()->addObject(new objects::ColorfulSpaceship(0, 0));
}

int main() {
  auto e = engine::Engine();

  firstScreen();
  e.run();
}
