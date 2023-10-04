#ifndef MESH_H
#define MESH_H

#include "Shader.h"

#include <vector>

namespace engine {
class Mesh {
public:
  Mesh(Shader *s, std::string name);
  ~Mesh(void);

  int getVAO(void);
  int getSize(void);

  const std::vector<std::pair<int, int>>& getTextureIndicies(void);

private:
  std::string name;
  int vao, vbo;
  int size;
  std::vector<std::pair<int, int>> texture_indicies;


  static std::map<std::string, std::pair<int, int>> texture_id_map;
  static std::map<std::string, std::tuple<int, int, int, std::vector<std::pair<int, int>>, int>> mesh_map;
};
} // namespace engine

#endif
