#ifndef OBJECT_H
#define OBJECT_H

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace engine {

class Object;
typedef std::tuple<int, int, int, int, Object*> drawable_t;

// class Object represents a thing that can be drawn using the engine, always
// having a shader associated. It has a shader name as it appears in
// ./res/shaders/ with the GLSL code that the object uses. This class is ment to
// be the base of all others more concrete objects in the engine.
class Object {
public:

  virtual ~Object(void);

  virtual void draw(int texture_id, int vao_id, int object_id) = 0;
  virtual void frame(void) = 0;

  const std::vector<drawable_t> &
  getDrawables(void);

protected:
  std::vector<drawable_t> drawables;
};
} // namespace engine

#endif
