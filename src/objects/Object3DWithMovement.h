#include "abstracts/Object3D.h"

#include "Engine.h"

#include <set>

extern "C" {
#include <GLFW/glfw3.h>
}

namespace engine {

class Object3DWithMovement : public Object3D {
private:
  int activator;
  bool skip_draw = true;
  static std::set<Object3DWithMovement *> objs;

public:
  Object3DWithMovement(std::string mesh_name, int activator);

  void frame(void) override;

  void draw(int texture_id, int vao_id, int object_id) override;
};
} // namespace engine
