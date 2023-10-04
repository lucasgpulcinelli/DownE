#include "ObjectFromFile.h"

#include "utils.h"

extern "C" {
#include <GL/glew.h>
}

#include <cstdio>
#include <fstream>
#include <sstream>

using namespace objects;

std::map<std::string, std::tuple<uint32_t, uint32_t, int>>
    ObjectFromFile::mesh_map;

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

} // namespace

std::pair<uint32_t, uint32_t> ObjectFromFile::loadMesh(void) {
  uint32_t vertex_array_id, vertex_buffer_id;

  const std::string mesh_path = "res/meshes/" + mesh_name + "/mesh.obj";
  std::vector<float> points = readMeshTris(mesh_path);
  mesh_size = points.size();

  glGenBuffers(1, &vertex_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

  glBufferData(GL_ARRAY_BUFFER, mesh_size * sizeof(float), points.data(),
               GL_STATIC_DRAW);

  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  GLint loc = glGetAttribLocation(properties[0], "point");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);

  loc = glGetAttribLocation(properties[0], "color");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(sizeof(float) * 3));

  return {vertex_array_id, vertex_buffer_id};
}

ObjectFromFile::ObjectFromFile(std::string shader_name, std::string mesh_name)
    : Object(shader_name) {

  this->mesh_name = mesh_name;

  if (mesh_map.count(mesh_name) != 0) {
    auto &t = mesh_map[mesh_name];
    properties.push_back(std::get<0>(t));
    std::get<2>(t)++;
    return;
  }

  debug("mesh " << mesh_name << " will be loaded");

  std::pair<uint32_t, uint32_t> vertex_props = loadMesh();
  properties.push_back(vertex_props.first);

  mesh_map[mesh_name] = {vertex_props.first, vertex_props.second, 1};
}

ObjectFromFile::~ObjectFromFile(void) {
  auto &t = mesh_map[mesh_name];

  if (std::get<2>(t)-- != 1) {
    return;
  }

  glDeleteVertexArrays(1, &std::get<0>(t));
  glDeleteBuffers(1, &std::get<1>(t));

  debug("mesh " << mesh_name << " will be deleted");

  mesh_map.erase(mesh_name);
}

void ObjectFromFile::draw(int w, int h) {
  Object::draw(w, h);

  glBindVertexArray(properties[1]);
}

int ObjectFromFile::getMeshSize(void) { return mesh_size; }
