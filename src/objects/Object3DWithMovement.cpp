#include "Object3DWithMovement.h"

using namespace engine;

std::set<Object3DWithMovement *> Object3DWithMovement::objs;

Object3DWithMovement::Object3DWithMovement(std::string mesh_name, int activator)
    : Object3D("simple3d", mesh_name) {
  this->activator = activator;
  objs.insert(this);
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
    scale += 0.01;
  }

  if (keys.find(GLFW_KEY_X) != keys.end()) {
    scale -= 0.01;
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
    draw_triangles = !draw_triangles;
  }

  if (keys.find(GLFW_KEY_V) != keys.end()) {
  }
}

void Object3DWithMovement::draw(int texture_id, int vao_id, int object_id) {
  if (!skip_draw) {
    Object3D::draw(texture_id, vao_id, object_id);
  }
}
