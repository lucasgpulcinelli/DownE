#ifndef TEXTURE_H
#define TEXTURE_H

#include <map>
#include <string>

namespace engine {
class Texture {
private:
  uint32_t id;
  std::string name;

  void loadTexture(std::string file);

  // the texture_id map, mapping texture names (a file in res/textures/) to its
  // id and count of instances.
  static std::map<std::string, std::pair<uint32_t, int>> texture_id_map;

public:
  Texture(std::string name);
  ~Texture(void);

  uint32_t getId(void) const;
};
} // namespace engine

#endif
