#include <eigen3/Eigen/Dense>

#include "Object3D.h"

#include "../CameraObject.h"
#include "../Skybox.h"
#include "Engine.h"
#include "WaveFrontObj.h"
#include "utils.h"

#include <cmath>

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

  auto w_h = Engine::getEngine()->getWindowSize();
  float ar = float(w_h.first) / w_h.second;

  loc = glGetUniformLocation(shader_id, "ar");
  glUniform1f(loc, ar);

  auto *c = CameraObject::getCamera();
  if (c != nullptr) {
    loc = glGetUniformLocation(shader_id, "camera_angle");
    const float *camera_angle = c->getViewAngle();
    glUniform2f(loc, camera_angle[0], camera_angle[1]);
  }

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

void Object3D::checkSkybox(void) {
  float hp = 0;
  auto bbox = m->getBoundingBox();

  Eigen::Matrix4f tr_s =
      (Eigen::Matrix4f() << scales[0], 0, 0, position[0], 0, scales[1], 0,
       position[1], 0, 0, scales[2], position[2], 0, 0, 0, 1)
          .finished();

  Eigen::Matrix4f rx =
      (Eigen::Matrix4f() << 1, 0, 0, 0, 0, cos(angles[0]), -sin(angles[0]), 0,
       0, sin(angles[0]), cos(angles[0]), 0, 0, 0, 0, 1)
          .finished();

  Eigen::Matrix4f ry =
      (Eigen::Matrix4f() << cos(angles[1]), 0, -sin(angles[1]), 0, 0, 1, 0, 0,
       sin(angles[1]), 0, cos(angles[1]), 0, 0, 0, 0, 1)
          .finished();

  // use i as a bit mask: if the bit is zero, use the minimum coordinate, else
  // use the maximum coordinate
  for (int i = 0; i < 8; i++) {
    float x, y, z;
    if (i & 0b001) {
      x = bbox[0];
    } else {
      x = bbox[3];
    }

    if (i & 0b010) {
      y = bbox[1];
    } else {
      y = bbox[4];
    }

    if (i & 0b100) {
      z = bbox[2];
    } else {
      z = bbox[5];
    }

    Eigen::Vector4f in_point(x, y, z, 1);
    Eigen::Vector4f out_point = tr_s * ry * rx * in_point;

    hp = std::max(hp, std::hypot(out_point.x(), out_point.y(), out_point.z()));
  }

  float skyboxr = Skybox::getSkyboxRadius();

  if (hp > skyboxr - 0.1) {
    for (int i = 0; i < 3; i++) {
      position[i] *= (skyboxr - 0.1) / hp;
    }
  }
}
