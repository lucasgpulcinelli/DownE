#ifndef MATERIAL_H
#define MATERIAL_H

#include <fstream>
#include <string>

namespace engine {

class Material {
private:
  std::string name;
  std::string texture_file;

public:
  Material(std::fstream &f);
  std::string getName(void) const;
  std::string getTextureFile(void) const;
};

} // namespace engine

#endif
