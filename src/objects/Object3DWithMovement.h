#include "Object3DWithLight.h"

#include "Engine.h"

#include <chrono>
#include <set>

extern "C" {
#include <GLFW/glfw3.h>
}

namespace engine {

// class Object3DWithMovement implements an object that should be activated with
// a certain key (which deactivate all others), and is controlled via wasd,
// arrow keys, and some other functionalities.
class Object3DWithMovement : public Object3DWithLight {
private:
  int activator;
  bool skip_logic = true;
  static std::set<Object3DWithMovement *> objs;
  int textures_filter = GL_LINEAR;
  std::chrono::time_point<std::chrono::high_resolution_clock> changed_mesh_time;

  bool boundBoxOk(void);

public:
  Object3DWithMovement(float position[3], std::string mesh_name, int activator);

  void frame(void) override;

  void draw(int texture_id, int vao_id, int object_id) override;
};
} // namespace engine
