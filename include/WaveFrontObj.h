#ifndef WAVE_FRONT_OBJ_H
#define WAVE_FRONT_OBJ_H

#include <string>
#include <vector>

#define FLOATS_PER_VERTEX 8

namespace engine {

class WaveFrontObj {
private:
  std::vector<float> tris;
  std::vector<float> verticies;
  std::vector<float> textures;
  std::vector<float> normals;

  std::vector<std::pair<std::string, int>> material_indicies;

  void storeFace(std::istringstream &stream);

public:
  WaveFrontObj(std::string file);

  const std::vector<float> &getTris(void) const;
  const std::vector<std::pair<std::string, int>> &
  getMaterialIndicies(void) const;
};

} // namespace engine

#endif
