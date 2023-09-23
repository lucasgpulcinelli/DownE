#ifndef COLORFUL_TRIANGLE_H
#define COLORFUL_TRIANGLE_H

#include "abstracts/Object.h"

namespace objects {
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
