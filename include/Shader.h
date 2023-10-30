#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>

namespace engine {

// class Shader manages shader compilation, deletion and reuse when necessary.
class Shader {
public:
  // creates a new shader with a certain name, compiling it if it does not exist
  // yet, and just getting its id if it does.
  // the name of the shader is defined as a directory in res/shaders, such that
  // each entry is a valid shader name. All directories inside res/shaders must
  // have files called vertex.glsl and fragment.glsl, with the shader code.
  Shader(std::string name);

  // deletes the shader, freeing it only if no other references to it exist.
  ~Shader(void);

  // gets the id of the shader
  int getShaderId(void);

private:
  int id;
  std::string name;

  // the shader map, mapping the name to the id and count of uses.
  static std::map<std::string, std::pair<int, int>> shader_map;
};
} // namespace engine

#endif
