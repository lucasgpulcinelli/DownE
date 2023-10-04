#include "ColorfulSpaceShip.h"

#include "Engine.h"
#include "utils.h"

#include <cmath>

extern "C" {
#include <GLFW/glfw3.h>
}

using namespace engine;

void ColorfulSpaceship::frame(void) {
  auto w_h = Engine::getEngine()->getWindowSize();
  auto r = (float)w_h.first / w_h.second;
  auto keys = Engine::getEngine()->getPressedKeys();

  if (keys.find(GLFW_KEY_UP) != keys.end()) {
    vx -= sin(angle) * 0.005 * scale;
    vy += cos(angle) * 0.005 * scale;
  }
  if (keys.find(GLFW_KEY_DOWN) != keys.end()) {
    vx += sin(angle) * 0.001 * scale*r;
    vy -= cos(angle) * 0.001 * scale;
  }
  if (keys.find(GLFW_KEY_LEFT) != keys.end()) {
    angle += 0.1;
  }
  if (keys.find(GLFW_KEY_RIGHT) != keys.end()) {
    angle -= 0.1;
  }
  if (keys.find(GLFW_KEY_EQUAL) != keys.end()) {
    scale += 0.05;
  }
  if (keys.find(GLFW_KEY_MINUS) != keys.end()) {
    scale -= 0.05;
  }

  x += vx;
  y += vy;

  if (x > 0.9*r) {
    x = 0.9*r;
    vx = 0;
  } else if (x < -0.9*r) {
    x = -0.9*r;
    vx = 0;
  }
  if (y > 0.9) {
    y = 0.9;
    vy = 0;
  } else if (y < -0.9) {
    y = -0.9;
    vy = 0;
  }

  if (scale < 0.1) {
    scale = 0.1;
  }
}
