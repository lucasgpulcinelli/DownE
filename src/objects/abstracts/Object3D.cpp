#include "Object3D.h"

#include "WaveFrontObj.h"
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

  auto material_i = m->getMaterialIndicies();
  debug("3d object got " << material_i.size() << " materials with a total of "
                         << m->getSize() / FLOATS_PER_VERTEX << " verticies");

  if (material_i.size() == 0) {
    return;
  }

  // for each texture, prepare the id_index_map for use in the draw method.
  int i;
  for (i = 0; i < (int)material_i.size() - 1; i++) {
    id_material_map[id_count] = {
        material_i[i].second, material_i[i + 1].second - material_i[i].second,
        material_i[i].first->getProperties()};

    drawables.push_back({shader_id, material_i[i].first->getTexture()->getId(),
                         m->getVAO(), id_count, this});
    id_count++;
  }

  // make sure we draw until the end of the object.
  id_material_map[id_count] = {material_i[i].second,
                               m->getSize() - material_i[i].second,
                               material_i[i].first->getProperties()};

  drawables.push_back({shader_id, material_i[i].first->getTexture()->getId(),
                       m->getVAO(), id_count, this});
  id_count++;

  debug("done creating 3d object, ids:");
  for (auto id_start_count : id_material_map) {
    debug("id " << id_start_count.first << ", start "
                << std::get<0>(id_start_count.second) << ", count "
                << std::get<1>(id_start_count.second));
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
  glUniform3f(loc, angles[0], angles[1], angles[2]);

  loc = glGetUniformLocation(shader_id, "scale");
  glUniform3f(loc, scales[0], scales[1], scales[2]);

  loc = glGetUniformLocation(shader_id, "position");
  glUniform3f(loc, position[0], position[1], position[2]);

  int start = std::get<0>(id_material_map[object_id]);
  int count = std::get<1>(id_material_map[object_id]);

  if (draw_triangles) {
    glDrawArrays(GL_TRIANGLES, start / FLOATS_PER_VERTEX,
                 count / FLOATS_PER_VERTEX);
    return;
  }

  for (int i = start / FLOATS_PER_VERTEX;
       i < start / FLOATS_PER_VERTEX + count / FLOATS_PER_VERTEX; i += 3) {

    glDrawArrays(GL_LINE_LOOP, i, 3);
  }
}
