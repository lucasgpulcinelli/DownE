#include "Material.h"

#include <fstream>
#include <map>
#include <sstream>
#include <string>

using namespace engine;

// readMaterialFile reads a mtl file and returns all important properties: the
// map from material names to texture filenames
Material::Material(std::fstream &f) {
  std::string s;

  f >> name;

  while (f >> s) {
    if (s == "newmtl") {
      break;
    }
    if (s == "map_Kd") {
      f >> texture_file;
    }
  };
}

std::string Material::getName(void) const { return name; }

std::string Material::getTextureFile(void) const { return texture_file; }
