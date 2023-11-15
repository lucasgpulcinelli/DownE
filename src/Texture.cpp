#include "Texture.h"

#include "utils.h"

extern "C" {
#include <GL/glew.h>

#define STBI_NO_FAILURE_STRINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
}

using namespace engine;

std::map<std::string, std::pair<uint32_t, int>> Texture::texture_id_map;

Texture::Texture(std::string name) {
  this->name = name;

  if (texture_id_map.find(name) != texture_id_map.end()) {
    auto &id_count = texture_id_map[name];
    id_count.second++;
    id = id_count.first;
    return;
  }

  loadTexture("res/textures/" + name);
  texture_id_map[name] = {id, 1};
}

Texture::~Texture(void) {
  auto &id_count = texture_id_map[name];

  if (id_count.second-- > 1) {
    return;
  }

  texture_id_map.erase(name);

  // delete texture
}

void Texture::loadTexture(std::string file) {
  int width, height, nr_channels;
  uint8_t *data = stbi_load(file.c_str(), &width, &height, &nr_channels, 0);

  debug("data has " << nr_channels << " channels");
  if (data == nullptr) {
    error(file << " returned null when tring to read");
  }

  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, (nr_channels == 3) ? GL_RGB : GL_RGBA, width,
               height, 0, (nr_channels == 3) ? GL_RGB : GL_RGBA,
               GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
}

uint32_t Texture::getId() const { return id; }
