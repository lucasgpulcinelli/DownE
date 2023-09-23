#include "Object.h"
#include "utils.h"

extern "C" {
#include <GL/glew.h>
}

#include <filesystem>

using namespace objects;

namespace fs = std::filesystem;

int Object::shader_now = -1;
std::map<std::string, std::pair<int, int>> Object::shader_map;

namespace {

GLenum getShaderType(const fs::directory_entry shader_file) {
  auto filename = shader_file.path().filename().string();

  if (filename == "vertex.glsl") {
    return GL_VERTEX_SHADER;
  }
  if (filename == "fragment.glsl") {
    return GL_FRAGMENT_SHADER;
  }

  error("invalid shader filename: " << filename);
}

char *readFileIntoString(const char *filename) {
  FILE *fptr = fopen(filename, "r");

  fseek(fptr, 0, SEEK_END);
  int size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  char *data = (char *)malloc(size);

  fread(data, 1, size, fptr);

  fclose(fptr);
  return data;
}

int compileShader(std::string shader_name) {
  const fs::path program_path = "res/shaders/" + shader_name;
  auto program_dir = fs::directory_entry(program_path);

  std::string program_name = program_dir.path().filename();

  GLuint program_id = glCreateProgram();

  for (const auto &shader_file : fs::directory_iterator(program_dir)) {
    GLenum shader_type = getShaderType(shader_file);

    char *source = readFileIntoString(shader_file.path().c_str());

    GLuint shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);

    GLint successful;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &successful);

    if (!successful) {
      GLint errlen = 0;
      glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &errlen);

      GLchar *error_log = (GLchar *)malloc(sizeof(GLchar) * errlen);
      glGetShaderInfoLog(shader_id, errlen, &errlen, error_log);

      debug(source);
      error("error compiling shader:" << std::endl << error_log);
    }

    glAttachShader(program_id, shader_id);

    free(source);
  }

  glLinkProgram(program_id);

  return program_id;
}

void freeShader(int shader_id) { glDeleteProgram(shader_id); }

} // namespace

Object::Object(std::string shader_name) {
  this->shader_name = shader_name;

  if (shader_map.count(shader_name) != 0) {
    auto &shader_count = shader_map[shader_name];
    properties.push_back(shader_count.first);
    shader_count.second++;
    return;
  }

  debug("shader " << shader_name << " will be compiled");

  int shader_id = compileShader(shader_name);
  properties.push_back(shader_id);
  shader_map[shader_name] = {shader_id, 1};
}

Object::~Object(void) {
  auto &shader_count = shader_map[shader_name];
  if (shader_count.second-- != 1) {
    return;
  }

  debug("shader " << shader_name << " will be deleted");

  freeShader(shader_count.first);
  shader_map.erase(shader_name);
}

void Object::draw(__attribute__((unused)) int w,
                  __attribute__((unused)) int h) {
  if (properties[0] != shader_now) {
    glUseProgram(properties[0]);
    shader_now = properties[0];
  }
}

const std::vector<int> Object::getProperties(void) { return properties; }
