#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include "WaveFrontMtl.h"

#include <vector>

namespace engine {

// class Mesh defines a 3D object as a wavefront object and material files, plus
// any texture files necessary for rendering. It creates a single VAO for the
// whole wavefront, containing X, Y, Z, U and V coordinates in order for each
// vertex, with three verticies defining each triangle such that a single
// glDrawArrays call is necessary for each texture.
class Mesh {
public:
  // creates the mesh with a certain shader and a certain name, where a valid
  // name is a directory in res/meshes, with the directory containing files
  // mesh.mtl and mesh.obj. If meshes with the same name are created, no new
  // opengl objects are created, everything is reused.
  Mesh(Shader *s, std::string name);

  // deletes the mesh, deleting the associated opengl objects if there are no
  // more references to them via other meshes.
  ~Mesh(void);

  // gets the VAO of the mesh.
  uint32_t getVAO(void);

  // gets the coordinate count of the mesh (such that getSize()/5 is the vertex
  // count).
  int getSize(void);

  // gets the texture indicies, meaning the vector of pairs where the first
  // element is the texture that should be used, and the second is the first
  // vertex that should have this texture applied.
  const std::vector<std::pair<int, int>> &getTextureIndicies(void);

  const std::vector<float> &getBoundingBox(void);

private:
  std::string name;
  uint32_t vao, vbo;
  int size;
  std::vector<std::pair<int, int>> texture_indicies;
  std::vector<float> bounding_box;

  WaveFrontMtl *wfmtl;

  void loadMesh(int shader_id);

  // the mesh map, mapping a mesh name to its properties: vao, vbo, size in
  // coordinates, texture_indicies vector, and count of instances.
  static std::map<std::string,
                  std::tuple<int, int, int, std::vector<std::pair<int, int>>,
                             std::vector<float>, int>>
      mesh_map;
};
} // namespace engine

#endif
