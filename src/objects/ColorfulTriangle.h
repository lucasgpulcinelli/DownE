#ifndef COLORFUL_TRIANGLE_H
#define COLORFUL_TRIANGLE_H

#include "abstracts/Object.h"

namespace objects {

// class ColorfulTriangle demonstrates what a generic object can do using a
// simple shader with color interpolation.
// ColorfulTriangles reuse the same vertex array id and buffers between all
// instances.
class ColorfulTriangle : public Object {
public:
  ColorfulTriangle(float x, float y, float scale = 1, float angle = 0);
  ~ColorfulTriangle(void);

  virtual void draw(int w, int h) override;

protected:
  float x, y, scale, angle;

private:
  static int count;
  static unsigned int vertex_array_id, vertex_buffer_id;
};
} // namespace objects

#endif
