#ifndef CUBE_H
#define CUBE_H

#include "abstracts/ObjectFromFile.h"
#include "utils.h"

extern "C" {
#include <GL/glew.h>
}

namespace objects {
// class Cube demonstrates an implementation of an ObjectFromFile object.
// TODO: flesh out shader and add more capabilities
class Cube : public ObjectFromFile {
public:
  Cube(void) : ObjectFromFile("simple3d", "cube") {}
  void draw(int w, int h) override {
    ObjectFromFile::draw(w, h);

    glDrawArrays(GL_TRIANGLES, 0, mesh_size/3);
  }
};
} // namespace objects

#endif
