#ifndef MATERIAL_H
#define MATERIAL_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "Texture.h"

namespace engine {

class Material {
private:
  std::string name;
  Texture *texture;
  std::map<std::string, std::vector<float>> properties_map;

public:
  Material(std::fstream &f);
  ~Material(void);

  std::string getName(void) const;
  const Texture *getTexture(void) const;
};

} // namespace engine

#endif
