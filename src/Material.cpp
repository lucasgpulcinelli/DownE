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

  std::map<std::string, std::pair<int, std::vector<float>>>
      material_properties_n = {{"Ns", {1, {1}}},       {"Ka", {3, {1, 1, 1}}},
                               {"Kd", {3, {1, 1, 1}}}, {"Ks", {3, {1, 1, 1}}},
                               {"Ke", {3, {1, 1, 1}}}, {"Ni", {1, {1}}},
                               {"d", {1, {1}}},        {"illum", {1, {1}}}};

  f >> name;

  while (f >> s) {
    if (s == "newmtl") {
      break;
    }
    if (s == "map_Kd") {
      f >> texture_file;
      continue;
    }

    int n = material_properties_n[s].first;

    if (n == 0) {
      error("error: invalid property " + s + " for material " + name);
    }

    std::string property_name = s;
    for (int i = 0; i < n; i++) {
      f >> s;
      properties_map[property_name].push_back(std::atof(s.c_str()));
    }
  };

  if (texture_file == "") {
    error("error: texture not found for material " + name);
  }

  for (auto prop_n : material_properties_n) {
    if (properties_map.find(prop_n.first) == properties_map.end()) {
      properties_map[prop_n.first] = prop_n.second.second;
    }
  }

  texture = new Texture(texture_file);
}

Material::~Material(void) { delete texture; }

std::string Material::getName(void) const { return name; }

const Texture *Material::getTexture(void) const { return texture; }

const std::map<std::string, std::vector<float>> *
Material::getProperties(void) const {
  return &properties_map;
}
