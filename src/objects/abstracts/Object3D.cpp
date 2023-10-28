#include "Object3D.h"

#include "utils.h"

extern "C" {
#include <GL/glew.h>
}

using namespace engine;

int Object3D::id_count = 0;

Object3D::Object3D(std::string shader_name, std::string mesh_name) {
  debug("starting 3d object creation");

  s = new Shader(shader_name);
  m = new Mesh(s, mesh_name);

  this->shader_id = s->getShaderId();

  auto tex_i = m->getTextureIndicies();
  debug("3d object got " << tex_i.size() << " textures with a total of "
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
  drawables.push_back({shader_id, tex_i[i].first, m->getVAO(), id_count, this});
  id_count++;


  debug("done creating 3d object, ids:");
  for (auto id_start_count : id_index_map) {
    debug("id " << id_start_count.first << ", start "
                << id_start_count.second.first << ", count "
                << id_start_count.second.second);
  }
}

Object3D::~Object3D(void) {
  delete m;
  delete s;
}

void Object3D::draw(int texture_id, int vao_id, int object_id) {
  if (texture_id == -1) {
    error("invalid texture for 3d object");
  }

  GLuint loc = glGetUniformLocation(shader_id, "angle");
  glUniform1f(loc, angles[0]);

  glDrawArrays(draw_triangles ? GL_TRIANGLES : GL_LINES,
               id_index_map[object_id].first / 5,
               id_index_map[object_id].second / 5);
}
