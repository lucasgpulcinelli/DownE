#ifndef WAVE_FRONT_MTL_H
#define WAVE_FRONT_MTL_H

#include <string>
#include <vector>

#include "Material.h"

namespace engine {

class WaveFrontMtl {
private:
  std::vector<Material> materials;

public:
  WaveFrontMtl(std::string file);
  const Material &getMaterial(std::string name) const;
};

} // namespace engine

#endif
