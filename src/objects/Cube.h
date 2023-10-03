#ifndef CUBE_H
#define CUBE_H

#include "Engine.h"
#include "abstracts/ObjectFromFile.h"
#include "utils.h"

extern "C" {
#include <GL/glew.h>
}

namespace objects {
// class Cube demonstrates an implementation of an ObjectFromFile object.
// TODO: flesh out shader and add more capabilities
class Cube : public ObjectFromFile {
private:
  float angle;

public:
  Cube(void) : ObjectFromFile("simple3d", "cube") {}
  void draw(int w, int h) override {
    ObjectFromFile::draw(w, h);

    auto &keys = engine::Engine::getEngine()->getPressedKeys();
    if (keys.find(GLFW_KEY_R) != keys.end()) {
      angle += 0.05;
    }

    GLuint loc = glGetUniformLocation(properties[0], "angle");
    glUniform1f(loc, angle);

    glDrawArrays(GL_TRIANGLES, 0, mesh_size / 3);
  }
};
} // namespace objects

#endif
