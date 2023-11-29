#include "Object3DWithLight.h"

#include "../LightObject.h"

extern "C" {
#include <GL/glew.h>
}

using namespace engine;

Object3DWithLight::Object3DWithLight(std::string shader_name,
                                     std::string mesh_name)
    : Object3D(shader_name, mesh_name) {}

void Object3DWithLight::draw(int texture_id, int vao_id, int object_id) {
  const float* light_position = LightObject::getPosition();
  const float* light_color = LightObject::getColor();

  GLuint loc = glGetUniformLocation(shader_id, "light_position");
  glUniform3f(loc, light_position[0], light_position[1], light_position[2]);

  loc = glGetUniformLocation(shader_id, "light_color");
  glUniform3f(loc, light_color[0], light_color[1], light_color[2]);

  Object3D::draw(texture_id, vao_id, object_id);
}

