#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <string>
#include <vector>

namespace objects {

// class Object represents a thing that can be drawn using the engine, always
// having a shader associated. It has a shader name as it appears in
// ./res/shaders/ with the GLSL code that the object uses. This class is ment to
// be the base of all others more concrete objects in the engine.
class Object {
public:
  // the constructor first sees if the shader with the matching name was already
  // compiled, if not, it reads the files
  // "./res/shaders/<shader_name>/vertex.glsl and
  // ./res/shaders/<shader_name>/fragment.glsl", compiling the shader and
  // populating drawing properties for it.
  Object(std::string shader_name);

  // the destructor deletes the shader if no other objects use it. This is
  // slower (not that much because of shader cache that most opengl
  // implementers, such as mesa, use) but more VRAM efficient.
  virtual ~Object(void);

  // draw draws the object in a screen with w width and h height in pixels.
  virtual void draw(int w, int h);

  // getProperties returns the drawing properties of this object, used by the
  // engine when adding new objects.
  const std::vector<int> getProperties(void);

protected:
  // the drawing properties of this object, such as shader id, vertex array
  // object or texture id.
  std::vector<int> properties;

private:
  // the shader name from the constructor, needed in the destructor.
  std::string shader_name;

  // the shader currently being used (as in the last shader passed as an
  // argument to glUseProgram), made to minimize calls to that function.
  // TODO: see if this is needed or opengl implementers already do this kind
  // of check as in mesa's glBindVertexArray.
  static int shader_now;

  // the shader map mappes shader names to the shader id and the number of
  // objects that use this shader, in that order.
  static std::map<std::string, std::pair<int, int>> shader_map;
};
} // namespace objects

#endif
