#ifndef CUBE_H
#define CUBE_H

#include "Engine.h"
#include "Mesh.h"
#include "Shader.h"
#include "abstracts/Object.h"
#include "utils.h"

extern "C" {
#include <GL/glew.h>
}

namespace engine {
// class Cube demonstrates an implementation of an ObjectFromFile object.
// TODO: flesh out shader and add more capabilities
class Cube : public Object {
private:
  float angle = 0;
  int shader_id;
  int mesh_size;

  Shader *s;
  Mesh *m;

public:
  Cube(void) {
    s = new Shader("simple3d");
    m = new Mesh(s, "cube");

    this->shader_id = s->getShaderId();
    this->mesh_size = m->getSize();

    drawables.push_back({shader_id, -1, m->getVAO(), this});
  }

  ~Cube(void) {
    delete m;
    delete s;
  }

  void frame(void) override {
    auto &keys = engine::Engine::getEngine()->getPressedKeys();
    if (keys.find(GLFW_KEY_R) != keys.end()) {
      angle += 0.05;
    }
  }

  void draw(int texture_id, int vao_id) override {
    GLuint loc = glGetUniformLocation(shader_id, "angle");
    glUniform1f(loc, angle);

    glDrawArrays(GL_TRIANGLES, 0, mesh_size / 3);
  }
};
} // namespace engine

#endif
