#include <eigen3/Eigen/Dense>

#include "Object3DWithMovement.h"

#include "utils.h"

using namespace engine;

std::set<Object3DWithMovement *> Object3DWithMovement::objs;

Object3DWithMovement::Object3DWithMovement(std::string mesh_name, int activator)
    : Object3DWithLight("simple3d", mesh_name) {
  this->activator = activator;
  objs.insert(this);

  changed_mesh_time = std::chrono::high_resolution_clock::now();
}

void Object3DWithMovement::frame(void) {
  auto &keys = Engine::getEngine()->getPressedKeys();

  if (keys.find(activator) != keys.end()) {
    for (auto *obj : objs) {
      if (obj == this) {
        continue;
      }
      obj->skip_draw = true;
    }
    this->skip_draw = false;
  }

  if (skip_draw) {
    return;
  }

  if (keys.find(GLFW_KEY_LEFT) != keys.end()) {
    angles[1] -= 0.05;
  }

  if (keys.find(GLFW_KEY_RIGHT) != keys.end()) {
    angles[1] += 0.05;
  }

  if (keys.find(GLFW_KEY_UP) != keys.end()) {
    angles[0] += 0.05;
  }

  if (keys.find(GLFW_KEY_DOWN) != keys.end()) {
    angles[0] -= 0.05;
  }

  if (keys.find(GLFW_KEY_Z) != keys.end()) {
    scales[0] += 0.01;
    scales[1] += 0.01;
    scales[2] += 0.01;
  }

  if (keys.find(GLFW_KEY_X) != keys.end()) {
    scales[0] -= 0.01;
    scales[1] -= 0.01;
    scales[2] -= 0.01;
  }

  if (keys.find(GLFW_KEY_W) != keys.end()) {
    position[1] += 0.05;
  }
  if (keys.find(GLFW_KEY_A) != keys.end()) {
    position[0] -= 0.05;
  }
  if (keys.find(GLFW_KEY_S) != keys.end()) {
    position[1] -= 0.05;
  }
  if (keys.find(GLFW_KEY_D) != keys.end()) {
    position[0] += 0.05;
  }
  if (keys.find(GLFW_KEY_SPACE) != keys.end()) {
    position[2] += 0.05;
  }
  if (keys.find(GLFW_KEY_B) != keys.end()) {
    position[2] -= 0.05;
  }

  if (keys.find(GLFW_KEY_P) != keys.end()) {
    auto time_now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> delta = time_now - changed_mesh_time;
    if (delta.count() < 0.5) {
      return;
    }

    changed_mesh_time = time_now;

    draw_triangles = !draw_triangles;
  }
}

bool Object3DWithMovement::boundBoxOk(void) {
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

  auto bbox = m->getBoundingBox();

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

    if (out_point.x() < -1 || out_point.x() > 1) {
      return false;
    }

    if (out_point.y() < -1 || out_point.y() > 1) {
      return false;
    }

    if (out_point.z() < -1 || out_point.z() > 1) {
      return false;
    }
  }

  return true;
}

void Object3DWithMovement::draw(int texture_id, int vao_id, int object_id) {
  if (skip_draw) {
    return;
  }

  Object3DWithLight::draw(texture_id, vao_id, object_id);

  auto &keys = Engine::getEngine()->getPressedKeys();

  // this functionality must be done here because the texture is already bound,
  // and doing so in the frame function would cause racing condition problems
  // (because draw and frame execute in different threads)
  if (keys.find(GLFW_KEY_V) != keys.end()) {
    auto time_now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> delta = time_now - changed_mesh_time;
    if (delta.count() < 0.5) {
      return;
    }

    changed_mesh_time = time_now;

    textures_filter = (textures_filter == GL_LINEAR) ? GL_NEAREST : GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textures_filter);
  }
}
