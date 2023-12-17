#include "Object3DWithLight.h"

#include "CameraObject.h"
#include "Engine.h"
#include "LightObject.h"
#include "utils.h"

#include <stdexcept>

extern "C" {
#include <GL/glew.h>
}

using namespace engine;

float Object3DWithLight::ambient_light_color[3] = {1, 1, 1};

Object3DWithLight::Object3DWithLight(float position[3], std::string shader_name,
                                     std::string mesh_name)
    : Object3D(position, shader_name, mesh_name) {}

void Object3DWithLight::draw(int texture_id, int vao_id, int object_id) {
  const float *light_position = LightObject::getPosition();
  const float *light_color = LightObject::getColor();

  GLuint loc = glGetUniformLocation(shader_id, "light_position");
  glUniform3f(loc, light_position[0], light_position[1], light_position[2]);

  loc = glGetUniformLocation(shader_id, "light_color");
  glUniform3f(loc, light_color[0], light_color[1], light_color[2]);

  loc = glGetUniformLocation(shader_id, "ambient_light_color");
  glUniform3f(loc, ambient_light_color[0], ambient_light_color[1],
              ambient_light_color[2]);

  CameraObject *c = CameraObject::getCamera();
  const float *cpos = c->getPosition();

  loc = glGetUniformLocation(shader_id, "view_position");
  glUniform3f(loc, cpos[0], cpos[1], cpos[2]);

  auto *properties_map = std::get<2>(id_material_map[object_id]);

  auto ka = properties_map->at("Ka");
  auto kd = properties_map->at("Kd");
  auto ks = properties_map->at("Ks");
  auto ns = properties_map->at("Ns");

  loc = glGetUniformLocation(shader_id, "ka");
  glUniform3f(loc, ka[0], ka[1], ka[2]);

  loc = glGetUniformLocation(shader_id, "kd");
  glUniform3f(loc, kd[0], kd[1], kd[2]);

  loc = glGetUniformLocation(shader_id, "ks");
  glUniform3f(loc, ks[0], ks[1], ks[2]);

  loc = glGetUniformLocation(shader_id, "ns");
  glUniform1f(loc, ns[0]);

  Object3D::draw(texture_id, vao_id, object_id);
}

void Object3DWithLight::frame(void) {
  auto &keys = Engine::getEngine()->getPressedKeys();

  if (keys.find(GLFW_KEY_R) != keys.end()) {
    ambient_light_color[0] += 0.05;
  }
  if (keys.find(GLFW_KEY_T) != keys.end()) {
    ambient_light_color[1] += 0.05;
  }
  if (keys.find(GLFW_KEY_Y) != keys.end()) {
    ambient_light_color[2] += 0.05;
  }
  if (keys.find(GLFW_KEY_U) != keys.end()) {
    ambient_light_color[0] -= 0.05;
  }
  if (keys.find(GLFW_KEY_I) != keys.end()) {
    ambient_light_color[1] -= 0.05;
  }
  if (keys.find(GLFW_KEY_O) != keys.end()) {
    ambient_light_color[2] -= 0.05;
  }
}
