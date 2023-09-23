#ifndef COLORFUL_SPACESHIP_H
#define COLORFUL_SPACESHIP_H

#include "ColorfulTriangle.h"

namespace objects {
class ColorfulSpaceship : public ColorfulTriangle {
public:
  ColorfulSpaceship(float x, float y, float scale = 1, float angle = 0)
      : ColorfulTriangle(x, y, scale, angle) {}
  void draw(int w, int h) override;

protected:
  float vx = 0, vy = 0;
};
} // namespace objects

#endif
