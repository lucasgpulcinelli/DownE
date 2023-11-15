#include "WaveFrontObj.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "utils.h"

using namespace engine;

// storeFace scans a single line in a wavefront file (in stream) where the first
// chacter is a face (already read at this point) and adds the coordinates and
// texture indicies to tries, reading from verticies and textures to do that.
void WaveFrontObj::storeFace(std::istringstream &stream) {
  std::vector<std::pair<int, int>> tri_indicies;
  int vi, vti, vni;
  std::string word;

  // read each num/num/num tuple
  while (stream >> word) {
    // we do not support %d//%d
    int read = std::sscanf(word.c_str(), "%d/%d/%d", &vi, &vti, &vni);
    if (read <= 0) {
      error("mesh file is incorrect");
    }
    if (read == 1) {
      error("loading meshes without textures not supported");
    }

    tri_indicies.push_back({vi - 1, vti - 1});
  }

  // for each tuple, make a triangle such that the whole line equates to a
  // polygon
  for (int i = 1; i < (int)tri_indicies.size() - 1; i++) {
    for (int j = 0; j < 3; j++) {
      tris.push_back(verticies[tri_indicies[0].first * 3 + j]);
    }
    tris.push_back(textures[tri_indicies[0].second * 2]);
    tris.push_back(textures[tri_indicies[0].second * 2 + 1]);

    for (int j = 0; j < 3; j++) {
      tris.push_back(verticies[tri_indicies[i].first * 3 + j]);
    }
    tris.push_back(textures[tri_indicies[i].second * 2]);
    tris.push_back(textures[tri_indicies[i].second * 2 + 1]);

    for (int j = 0; j < 3; j++) {
      tris.push_back(verticies[tri_indicies[i + 1].first * 3 + j]);
    }
    tris.push_back(textures[tri_indicies[i + 1].second * 2]);
    tris.push_back(textures[tri_indicies[i + 1].second * 2 + 1]);
  }
}

// readMeshTris reads all tris in a file in the wavefont obj format and return a
// vector of vertex coordinates and texels and all material name and indicies of
// the object, ordered for use with glDrawArrays in the GL_TRIANGLES mode.
WaveFrontObj::WaveFrontObj(std::string file) {
  debug("start mesh reading for " << file);
  std::fstream f(file, std::ios_base::in);

  // for each line
  std::string line;
  while (std::getline(f, line)) {
    std::istringstream stream(line);

    std::string s;
    stream >> s;
    if (s == "v") {
      // read a vertex into the verticies array

      float x, y, z;
      stream >> x >> y >> z;
      verticies.push_back(x);
      verticies.push_back(y);
      verticies.push_back(z);
    } else if (s == "usemtl") {
      // read the material name and store it

      std::string texture_name;
      stream >> texture_name;

      debug("changed mtl to " << texture_name << " at vertex "
                              << tris.size() / 5);
      material_indicies.push_back({texture_name, tris.size()});
    } else if (s == "vt") {
      // read the texel coordinates and store it in the textures array

      float u, v;
      stream >> u >> v;
      textures.push_back(u);
      textures.push_back(1 - v);
    } else if (s == "f") {
      // read the face indicies and store them in the final tries array
      storeFace(stream);
    }
  }

  f.close();

  debug("done reading mesh, got " << tris.size() / 5 << " verticies and "
                                  << material_indicies.size() << " materials");
}

const std::vector<float> &WaveFrontObj::getTris(void) const { return tris; }

const std::vector<std::pair<std::string, int>> &
WaveFrontObj::getMaterialIndicies(void) const {
  return material_indicies;
}
