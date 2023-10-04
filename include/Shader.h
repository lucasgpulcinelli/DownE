#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>

namespace engine {
class Shader {
public:
  Shader(std::string name);
  ~Shader(void);

  int getShaderId(void);

private:
  int id;
  std::string name;

  static std::map<std::string, std::pair<int, int>> shader_map;
};
} // namespace engine

#endif
