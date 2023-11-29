#include "Material.h"

#include "Texture.h"
#include "utils.h"

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace engine;

// readMaterialFile reads a mtl file and sets all important properties: the
// map from material names to texture filenames
Material::Material(std::fstream &f) {
  std::string s;
  std::string texture_file = "";

  std::map<std::string, int> material_properties_n = {
      {"Ns", 1}, {"Ka", 3}, {"Kd", 3}, {"Ks", 3},
      {"Ke", 3}, {"Ni", 1}, {"d", 1},  {"illum", 1}};

  f >> name;

  while (f >> s) {
    if (s == "newmtl") {
      break;
    }
    if (s == "map_Kd") {
      f >> texture_file;
      continue;
    }

    int n = material_properties_n[s];

    if (n == 0) {
      error("error: invalid property " + s + " for material " + name);
    }

    for (int i = 0; i < n; i++) {
      f >> s;
      properties_map[s].push_back(std::atof(s.c_str()));
    }
  };

  if (texture_file == "") {
    error("error: texture not found for material " + name);
  }

  texture = new Texture(texture_file);
}

Material::~Material(void) { delete texture; }

std::string Material::getName(void) const { return name; }

const Texture *Material::getTexture(void) const { return texture; }
