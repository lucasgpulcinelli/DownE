#include "Mesh.h"
#include "utils.h"

#include <fstream>
#include <sstream>
#include <vector>

extern "C" {
#include <GL/glew.h>
}

using namespace engine;

namespace {
void storeFace(std::vector<float> &tris, std::vector<float> &verticies,
               std::vector<float> &textures, std::istringstream &stream) {
  std::vector<std::pair<int, int>> tri_indicies;
  int vi, vti, vni;
  std::string word;

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
// vector of vertex coordinates of the object, ordered for use with glDrawArrays
// in the GL_TRIANGLES mode.
std::vector<float> readMeshTris(std::string mesh_path) {
  std::fstream f(mesh_path, std::ios_base::in);

  std::vector<float> verticies;
  std::vector<float> textures;
  std::vector<float> tris;

  std::string line;
  while (std::getline(f, line)) {
    std::istringstream stream(line);

    std::string s;
    stream >> s;
    if (s == "v") {
      float x, y, z;
      stream >> x >> y >> z;
      verticies.push_back(x);
      verticies.push_back(y);
      verticies.push_back(z);
    } else if (s == "vt") {
      float u, v;
      stream >> u >> v;
      textures.push_back(u);
      textures.push_back(v);
    } else if (s == "f") {
      storeFace(tris, verticies, textures, stream);
    }
  }

  f.close();

  return tris;
}

std::tuple<int, int, int> loadMesh(int shader_id, std::string name) {
  uint32_t vao, vbo;

  const std::string mesh_path = "res/meshes/" + name + "/mesh.obj";
  std::vector<float> points = readMeshTris(mesh_path);
  int size = points.size();

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), points.data(),
               GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLint loc = glGetAttribLocation(shader_id, "point");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);

  loc = glGetAttribLocation(shader_id, "color");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(sizeof(float) * 3));

  return {vao, vbo, size};
}
} // namespace

std::map<std::string, std::tuple<int, int, int, int>> Mesh::mesh_map;

Mesh::Mesh(Shader *s, std::string name) {
  this->name = name;

  if (mesh_map.count(name) != 0) {
    auto &t = mesh_map[name];
    vao = std::get<0>(t);
    vbo = std::get<1>(t);
    size = std::get<2>(t);
    std::get<3>(t)++;
    return;
  }

  debug("mesh " << name << " will be loaded");

  auto properties = loadMesh(s->getShaderId(), name);
  vao = std::get<0>(properties);
  vbo = std::get<1>(properties);
  size = std::get<2>(properties);

  mesh_map[name] = {vao, vbo, size, 1};
}

int Mesh::getVAO(void) { return vao; }

int Mesh::getSize(void){return size;}

Mesh::~Mesh() {
  auto &t = mesh_map[name];

  if (std::get<2>(t)-- != 1) {
    return;
  }

  glDeleteVertexArrays(1, (uint32_t*)&std::get<0>(t));
  glDeleteBuffers(1, (uint32_t*)&std::get<1>(t));

  debug("mesh " << name << " will be deleted");

  mesh_map.erase(name);
}
