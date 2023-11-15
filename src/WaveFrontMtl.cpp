#include "WaveFrontMtl.h"

#include "Material.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace engine;

WaveFrontMtl::WaveFrontMtl(std::string file) {
  std::fstream f(file, std::ios_base::in);

  std::string s;
  while (f >> s) {
    if (s == "newmtl") {
      break;
    }
  }

  while (!f.eof()) {
    materials.push_back(Material(f));
  }
}

const Material &WaveFrontMtl::getMaterial(std::string name) const {
  for (auto &m : materials) {
    if (m.getName() == name) {
      return m;
    }
  }

  throw std::invalid_argument("no such material " + name);
}
