#include "ColorfulTriangle.h"
#include "Engine.h"
#include "utils.h"

extern "C" {
#include <GL/glew.h>
}

#include <cmath>

using namespace engine;

int ColorfulTriangle::count = 0;
unsigned int ColorfulTriangle::vertex_array_id = -1,
             ColorfulTriangle::vertex_buffer_id = -1;
int ColorfulTriangle::shader_id = -1;

Shader *ColorfulTriangle::s = nullptr;

ColorfulTriangle::ColorfulTriangle(float x, float y, float scale, float angle) {

  debug("creating colorful triangle");

  this->x = x;
  this->y = y;
  this->scale = scale;
  this->angle = angle;

  if (count++ != 0) {
    this->drawables.push_back({shader_id, -1, vertex_array_id, this});
    return;
  }

  debug("mesh properties will be created");

  s = new Shader("colorfulMesh");
  shader_id = s->getShaderId();

  glGenBuffers(1, &vertex_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

  // hardcoded regular triangle with upwards vertex in red.
  float points[] = {0,    0.5326920704511053, 1, 0, 0, 0.5, -1.0 / 3, 0, 1, 0,
                    -0.5, -1.0 / 3,           0, 0, 1};

  glBufferData(GL_ARRAY_BUFFER, 5 * 3 * sizeof(float), points, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  GLint loc = glGetAttribLocation(shader_id, "point");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);

  loc = glGetAttribLocation(shader_id, "color");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));

  this->drawables.push_back({shader_id, -1, vertex_array_id, this});
}

ColorfulTriangle::~ColorfulTriangle(void) {
  debug("deleting colorful triangle");
  if (count-- != 1) {
    return;
  }

  debug("mesh properties will be deleted");

  glDeleteVertexArrays(1, &vertex_array_id);
  glDeleteBuffers(1, &vertex_buffer_id);

  delete s;
}

void ColorfulTriangle::draw(int texture_id, int vao_id) {
  auto w_h = Engine::getEngine()->getWindowSize();
  float r = (float)w_h.second / w_h.first;

  float mat_scale[] = {scale, 0, 0, scale};
  float mat_rotation[] = {std::cos(angle), -std::sin(angle), std::sin(angle),
                          std::cos(angle)};
  float mat_translation[] = {1.0f*r, 0, 0, x*r, 0, 1.0f, 0, y,
                             0,    0, 1, 0, 0, 0,    0, 1};

  GLuint loc = glGetUniformLocation(shader_id, "mat_scale");
  glUniformMatrix2fv(loc, 1, GL_TRUE, mat_scale);

  loc = glGetUniformLocation(shader_id, "mat_rotation");
  glUniformMatrix2fv(loc, 1, GL_TRUE, mat_rotation);

  loc = glGetUniformLocation(shader_id, "mat_translation");
  glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation);

  debug(r);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
}
