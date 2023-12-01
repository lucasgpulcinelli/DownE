#include "Mesh.h"
#include "WaveFrontMtl.h"
#include "WaveFrontObj.h"
#include "utils.h"

#include <fstream>
#include <limits>
#include <sstream>
#include <vector>

extern "C" {
#include <GL/glew.h>
}

using namespace engine;

// loadMesh creates a whole new mesh from a mesh name, using a certain
// shader.
// the method sets VAO and VBO ids, the mesh size, the relation between
// textures and verticies indicies, and the bounding box for the object.
void Mesh::loadMesh(int shader_id) {
  const std::string mesh_path = "res/meshes/" + name + "/mesh.obj";
  WaveFrontObj wfo(mesh_path);

  const std::string mtl_path = "res/meshes/" + name + "/mesh.mtl";
  wfmtl = new WaveFrontMtl(mtl_path);

  // for each material, add a texture index entry
  for (auto mat_i : wfo.getMaterialIndicies()) {
    const Material *m = wfmtl->getMaterial(mat_i.first);

    material_indicies.push_back({m, mat_i.second});
  }

  auto points = wfo.getTris();
  size = points.size();

  // calculate the bounding box, first the min 3 coordinates then the max 3
  // coordinates
  bounding_box = std::vector<float>(6, std::numeric_limits<float>::max());
  bounding_box[3] = 0;
  bounding_box[4] = 0;
  bounding_box[5] = 0;

  for (int i = 0; i < size; i += FLOATS_PER_VERTEX) {
    bounding_box[0] = std::min(bounding_box[0], points[i]);
    bounding_box[1] = std::min(bounding_box[1], points[i + 1]);
    bounding_box[2] = std::min(bounding_box[2], points[i + 2]);
    bounding_box[3] = std::max(bounding_box[3], points[i]);
    bounding_box[4] = std::max(bounding_box[4], points[i + 1]);
    bounding_box[5] = std::max(bounding_box[5], points[i + 2]);
  }

  // create the vbo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // set the vbo with the data
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), points.data(),
               GL_STATIC_DRAW);

  // create the vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // set the vertex coordinates and texels with the correct data from the vbo

  GLint loc = glGetAttribLocation(shader_id, "point");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                        FLOATS_PER_VERTEX * sizeof(float), NULL);

  loc = glGetAttribLocation(shader_id, "texture");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE,
                        FLOATS_PER_VERTEX * sizeof(float),
                        (void *)(sizeof(float) * 3));

  loc = glGetAttribLocation(shader_id, "normal");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                        FLOATS_PER_VERTEX * sizeof(float),
                        (void *)(sizeof(float) * 5));
}

// vao, vbo, mesh size, material indicies and starting vertex, and count
std::map<std::string, std::tuple<int, int, int,
                                 std::vector<std::pair<const Material *, int>>,
                                 std::vector<float>, int>>
    Mesh::mesh_map;

Mesh::Mesh(Shader *s, std::string name) {
  debug("creating mesh for " << name);
  this->name = name;

  if (mesh_map.count(name) != 0) {
    auto &t = mesh_map[name];
    vao = std::get<0>(t);
    vbo = std::get<1>(t);
    size = std::get<2>(t);
    material_indicies = std::get<3>(t);
    bounding_box = std::get<4>(t);
    std::get<5>(t)++;
    return;
  }

  debug("mesh " << name << " will be loaded");

  loadMesh(s->getShaderId());

  mesh_map[name] = {vao, vbo, size, material_indicies, bounding_box, 1};
}

uint32_t Mesh::getVAO(void) { return vao; }

const std::vector<std::pair<const Material *, int>> &
Mesh::getMaterialIndicies(void) {
  return material_indicies;
}

int Mesh::getSize(void) { return size; }

Mesh::~Mesh() {
  auto &t = mesh_map[name];

  if (std::get<5>(t)-- != 1) {
    return;
  }

  debug("mesh " << name << " will be deleted");

  delete wfmtl;

  glDeleteVertexArrays(1, (uint32_t *)&std::get<0>(t));
  glDeleteBuffers(1, (uint32_t *)&std::get<1>(t));

  mesh_map.erase(name);
}

const std::vector<float> &Mesh::getBoundingBox(void) { return bounding_box; }
