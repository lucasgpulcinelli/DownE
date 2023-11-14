#ifndef WAVE_FRONT_OBJ_H
#define WAVE_FRONT_OBJ_H

#include <string>
#include <vector>

namespace engine {

class WaveFrontObj {
private:
  std::vector<float> tris;
  std::vector<float> verticies;
  std::vector<float> textures;

  std::vector<std::pair<std::string, int>> material_indicies;

  void storeFace(std::istringstream &stream);

public:
  WaveFrontObj(std::string file);

  const std::vector<float> &getTris(void);
  const std::vector<std::pair<std::string, int>> &getMaterialIndicies(void);
};

} // namespace engine

#endif
