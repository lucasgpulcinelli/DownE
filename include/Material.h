#ifndef MATERIAL_H
#define MATERIAL_H

#include <fstream>
#include <string>

#include "Texture.h"

namespace engine {

class Material {
private:
  std::string name;
  Texture *texture;

public:
  Material(std::fstream &f);
  ~Material(void);

  std::string getName(void) const;
  const Texture *getTexture(void) const;
};

} // namespace engine

#endif
