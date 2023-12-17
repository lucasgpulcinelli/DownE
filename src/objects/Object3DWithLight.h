#ifndef OBJECT_3D_WITH_LIGHT_H
#define OBJECT_3D_WITH_LIGHT_H

#include "abstracts/Object3D.h"

namespace engine {
class Object3DWithLight : public Object3D {
private:
  static float ambient_light_color[3];

public:
  Object3DWithLight(std::string shader_name, std::string mesh_name);

  void frame(void) override;
  void draw(int texture_id, int vao_id, int object_id) override;
};
} // namespace engine

#endif
