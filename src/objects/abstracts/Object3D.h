#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include "Material.h"
#include "Mesh.h"
#include "Object.h"
#include "Shader.h"

#include <map>

namespace engine {
// class Object3D implements an object with many textures.
class Object3D : public Object {
private:
  static int id_count;

protected:
  std::map<int, std::tuple<int, int,
                           const std::map<std::string, std::vector<float>> *>>
      id_material_map;

  int shader_id;

  Shader *s;
  Mesh *m;

  float angles[3] = {0, 0, 0};
  float position[3] = {0, 0, 0};
  float scales[3] = {1, 1, 1};

  // if the object should be drawn as triangles or lines. Note that drawing
  // lines is less efficient because of the Mesh VAO layout.
  bool draw_triangles = true;

public:
  Object3D(std::string shader_name, std::string mesh_name);
  ~Object3D();

  void draw(int texture_id, int vao_id, int object_id) override;
};
} // namespace engine

#endif
