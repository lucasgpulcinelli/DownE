#include "Object3DWithMovement.h"

#include "utils.h"

using namespace engine;

std::set<Object3DWithMovement *> Object3DWithMovement::objs;

Object3DWithMovement::Object3DWithMovement(std::string mesh_name, int activator)
    : Object3D("simple3d", mesh_name) {
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
    angles[0] -= 0.05;
  }

  if (keys.find(GLFW_KEY_RIGHT) != keys.end()) {
    angles[0] += 0.05;
  }

  if (keys.find(GLFW_KEY_UP) != keys.end()) {
    angles[1] += 0.05;
  }

  if (keys.find(GLFW_KEY_DOWN) != keys.end()) {
    angles[1] -= 0.05;
  }

  if (keys.find(GLFW_KEY_Z) != keys.end()) {
    scales[0] += 0.01;
  }

  if (keys.find(GLFW_KEY_X) != keys.end()) {
    scales[0] -= 0.01;
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

void Object3DWithMovement::draw(int texture_id, int vao_id, int object_id) {
  if (skip_draw) {
    return;
  }

  Object3D::draw(texture_id, vao_id, object_id);

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

    textures_filter = (textures_filter == GL_LINEAR)? GL_NEAREST : GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textures_filter);
  }
}
