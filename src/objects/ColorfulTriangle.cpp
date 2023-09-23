#include "ColorfulTriangle.h"
#include "utils.h"

extern "C" {
#include <GL/glew.h>
}

#include <cmath>

using namespace objects;

int ColorfulTriangle::count = 0;
unsigned int ColorfulTriangle::vertex_array_id = -1,
             ColorfulTriangle::vertex_buffer_id = -1;

ColorfulTriangle::ColorfulTriangle(float x, float y, float scale, float angle)
    : Object("colorfulMesh") {

  debug("creating colorful triangle");

  this->x = x;
  this->y = y;
  this->scale = scale;
  this->angle = angle;

  this->properties.push_back(count);
  if (count++ != 0) {
    return;
  }

  debug("mesh properties will be created");

  glGenBuffers(1, &vertex_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

  float points[] = {0,    0.5326920704511053, 1, 0, 0, 0.5, -1.0 / 3, 0, 1, 0,
                    -0.5, -1.0 / 3,           0, 0, 1};

  glBufferData(GL_ARRAY_BUFFER, 5 * 3 * sizeof(float), points, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  GLint loc = glGetAttribLocation(properties[0], "point");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);

  loc = glGetAttribLocation(properties[0], "color");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));
}

ColorfulTriangle::~ColorfulTriangle(void) {
  Object::~Object();

  debug("deleting colorful triangle");
  if (count-- != 1) {
    return;
  }

  debug("mesh properties will be deleted");

  glDeleteVertexArrays(1, &vertex_array_id);
  glDeleteBuffers(1, &vertex_buffer_id);
}

void ColorfulTriangle::draw(int w, int h) {
  Object::draw(w, h);

  float mat_scale[] = {scale, 0, 0, scale};
  float mat_rotation[] = {std::cos(angle), -std::sin(angle), std::sin(angle),
                          std::cos(angle)};
  float mat_translation[] = {
      1.0f * h / w, 0, 0, x * h / w, 0, 1.0f, 0, y, 0, 0, 1, 0, 0, 0, 0, 1};

  glBindVertexArray(vertex_array_id);

  GLuint loc = glGetUniformLocation(properties[0], "mat_scale");
  glUniformMatrix2fv(loc, 1, GL_TRUE, mat_scale);

  loc = glGetUniformLocation(properties[0], "mat_rotation");
  glUniformMatrix2fv(loc, 1, GL_TRUE, mat_rotation);

  loc = glGetUniformLocation(properties[0], "mat_translation");
  glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
}
