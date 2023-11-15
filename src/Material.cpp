#include "Material.h"

#include "Texture.h"

#include <fstream>
#include <map>
#include <sstream>
#include <string>

using namespace engine;

// readMaterialFile reads a mtl file and sets all important properties: the
// map from material names to texture filenames
Material::Material(std::fstream &f) {
  std::string s;
  std::string texture_file;

  f >> name;

  while (f >> s) {
    if (s == "newmtl") {
      break;
    }
    if (s == "map_Kd") {
      f >> texture_file;
    }
  };

  texture = new Texture(texture_file);
}

Material::~Material(void) { delete texture; }

std::string Material::getName(void) const { return name; }

const Texture *Material::getTexture(void) const { return texture; }
