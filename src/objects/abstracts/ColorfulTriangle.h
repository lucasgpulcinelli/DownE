#ifndef COLORFUL_TRIANGLE_H
#define COLORFUL_TRIANGLE_H

#include "Object.h"
#include "Shader.h"

namespace engine {

// class ColorfulTriangle demonstrates what a generic object can do using a
// simple shader with color interpolation.
// ColorfulTriangles reuse the same vertex array id and buffers between all
// instances.
class ColorfulTriangle : public Object {
public:
  ColorfulTriangle(float x, float y, float scale = 1, float angle = 0);
  ~ColorfulTriangle(void);

  void draw(int texture_id, int vao_id, int object_id) override;

protected:
  float x, y, scale, angle;

private:
  static Shader *s;
  static int shader_id;
  static int count;
  static unsigned int vertex_array_id, vertex_buffer_id;
};
} // namespace engine

#endif
