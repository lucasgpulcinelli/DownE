#ifndef COLORFUL_SPACESHIP_H
#define COLORFUL_SPACESHIP_H

#include "abstracts/ColorfulTriangle.h"

namespace engine {
// class ColorfulSpaceship demonstrates how the object oriented design of the
// engine is userful in creating actual game objects. It creates a spaceship
// that can be controlled via the arrow keys by the user. and has physics
// similar to the "Asteroids" arcade game.
// TODO: the user's screen refresh rate or current fps affects the speed in
// which this code is ran. The ideal solution is add another method and manage
// game interactions and drawing in separate threads.
class ColorfulSpaceship : public ColorfulTriangle {
public:
  ColorfulSpaceship(float x, float y, float scale = 1, float angle = 0)
      : ColorfulTriangle(x, y, scale, angle) {}

  void frame(void) override;

protected:
  float vx = 0, vy = 0;
};
} // namespace engine

#endif
