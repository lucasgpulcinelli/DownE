#ifndef OBJECT_3D_WITH_LIGHT_H
#define OBJECT_3D_WITH_LIGHT_H

#include "Object3D.h"

namespace engine {
class Object3DWithLight : public engine::Object3D {
  public:
  Object3DWithLight(std::string shader_name, std::string mesh_name);

  void draw(int texture_id, int vao_id, int object_id) override;
};
} // namespace engine

#endif
