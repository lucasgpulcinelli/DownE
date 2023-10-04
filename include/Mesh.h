#ifndef MESH_H
#define MESH_H

#include "Shader.h"

namespace engine {
class Mesh {
public:
  Mesh(Shader *s, std::string name);
  ~Mesh(void);

  int getVAO(void);
  int getSize(void);

private:
  std::string name;
  int vao, vbo;
  int size;

  static std::map<std::string, std::tuple<int, int, int, int>> mesh_map;
};
} // namespace engine

#endif
