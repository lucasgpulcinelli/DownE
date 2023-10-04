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
  static int id_count;

  float angle = 0;
  int shader_id;
  std::map<int, std::pair<int, int>> id_index_map;

  Shader *s;
  Mesh *m;

public:
  Cube(void) {
    debug("starting cube creation");

    s = new Shader("simple3d");
    m = new Mesh(s, "cube");

    this->shader_id = s->getShaderId();

    auto tex_i = m->getTextureIndicies();
    debug("cube got " << tex_i.size() << " textures with a total of "
                      << m->getSize() / 5 << " verticies");

    if (tex_i.size() == 0) {
      return;
    }

    int i;
    for (i = 0; i < (int)tex_i.size() - 1; i++) {
      id_index_map[id_count] = {tex_i[i].second,
                                tex_i[i + 1].second - tex_i[i].second};

      drawables.push_back(
          {shader_id, tex_i[i].first, m->getVAO(), id_count, this});
      id_count++;
    }

    id_index_map[id_count] = {tex_i[i].second, m->getSize() - tex_i[i].second};
    drawables.push_back(
        {shader_id, tex_i[i].first, m->getVAO(), id_count, this});
    id_count++;

    debug("done creating cube, ids:");
    for (auto id_start_count : id_index_map) {
      debug("id " << id_start_count.first << ", start "
                  << id_start_count.second.first << ", count "
                  << id_start_count.second.second);
    }
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

  void draw(int texture_id, int vao_id, int object_id) override {
    if (texture_id == -1) {
      error("invalid texture for cube");
    }

    GLuint loc = glGetUniformLocation(shader_id, "angle");
    glUniform1f(loc, angle);

    glDrawArrays(GL_TRIANGLES, id_index_map[object_id].first / 5,
                 id_index_map[object_id].second / 5);
  }
};

int Cube::id_count = 0;
} // namespace engine

#endif
