#ifndef OBJECT_H
#define OBJECT_H

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace engine {

class Object;
// drawable type as it appears in the engine, explained in detail in the include
// file for the engine itself.
typedef std::tuple<int, int, int, int, Object *> drawable_t;

// class Object represents a thing that has logic (via the frame method) and
// drawable components. This class is ment to be the base of all others more
// concrete objects in the engine.
class Object {
public:
  virtual ~Object(void);

  // draw draws one of the drawables of the object, with a certain texture, vao
  // and object internal id. It should be assumed that the shader, texture and
  // VAO are already setted up.
  virtual void draw(int texture_id, int vao_id, int object_id) = 0;

  // frame executes actions for the logical object at every frame.
  virtual void frame(void) = 0;

  // getDrawables
  const std::vector<drawable_t> &getDrawables(void);

protected:
  std::vector<drawable_t> drawables;
};
} // namespace engine

#endif
