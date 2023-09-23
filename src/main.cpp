#include "Engine.h"
#include "utils.h"

#include "objects/ColorfulSpaceShip.h"

void firstScreen(void) {
  debug("adding initial elements");
}

int main() {
  auto e = engine::Engine();

  firstScreen();
  e.run();
}
