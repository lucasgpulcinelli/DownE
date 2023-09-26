#ifndef OBJECT_FROM_FILE
#define OBJECT_FROM_FILE

#include "Object.h"

#include <cinttypes>

namespace objects {
// ObjectFromFile is an object that has a mesh in the wavefont obj format
// related to it. The shader for any object that uses this class as a base is
// assumed to have a "point" vec3 attribute. ObjectFromFile must be drawn using
// the GL_TRIANGLES mode.
class ObjectFromFile : public Object {
public:
  // the constructor reads the mesh name and creates a VAO and VBO for its data
  // if the mesh was never read (the shader is passed to the Object
  // constructor).
  ObjectFromFile(std::string shader_name, std::string mesh_name);

  // the destructor deletes the VAO and VBO for this object if no other objects
  // use such mesh.
  virtual ~ObjectFromFile(void);

  virtual void draw(int w, int h) override;

  // getMeshSize gets the mesh size in amount of point coordinates (such that if
  // you divide by 3 you have the amount of verticies, and by 9 you have the
  // amount of triangles)
  int getMeshSize(void);

protected:
  int mesh_size;

private:
  // loadMesh loads a new mesh uses the mesh name and creates a VAO and VBO for
  // it.
  std::pair<uint32_t, uint32_t> loadMesh(void);

  // the mesh name
  std::string mesh_name;

  // the mesh map, mapping mesh names to a tuple contaning VAO, VBO and count of
  // objects that use this mesh, in that order
  static std::map<std::string, std::tuple<uint32_t, uint32_t, int>> mesh_map;
};
} // namespace objects

#endif
