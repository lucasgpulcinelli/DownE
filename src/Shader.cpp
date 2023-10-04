#include "Shader.h"
#include "utils.h"

extern "C" {
#include <GL/glew.h>
}

#include <filesystem>

using namespace engine;

namespace fs = std::filesystem;

std::map<std::string, std::pair<int, int>> Shader::shader_map;

namespace {

// getShaderType returns the opengl type of the shader in the file using its
// filename.
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

// readFileIntoString reads a complete file and returns a NULL terminated
// character pointer to it. This pointer should be freed with C style free.
char *readFileIntoString(const char *filename) {
  FILE *fptr = fopen(filename, "r");

  fseek(fptr, 0, SEEK_END);
  int size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  char *data = (char *)malloc(size + 1);

  fread(data, 1, size, fptr);
  data[size] = '\0';

  fclose(fptr);
  return data;
}

// compileShader compiles a shader and returns its id, erroring out if any
// problems happen.
int compileShader(std::string shader_name) {
  const fs::path program_path = "res/shaders/" + shader_name;

  // read the shader directory
  auto program_dir = fs::directory_entry(program_path);

  std::string program_name = program_dir.path().filename();

  // create our new program that will be returned
  GLuint program_id = glCreateProgram();

  // for each shader source code
  for (const auto &shader_file : fs::directory_iterator(program_dir)) {
    // get its type
    GLenum shader_type = getShaderType(shader_file);

    // read the code
    char *source = readFileIntoString(shader_file.path().c_str());

    // create the shader of that code and compile it
    GLuint shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);

    // see if we were able to compile it correctly
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

    // attach the shader to the main program
    glAttachShader(program_id, shader_id);

    free(source);
  }

  // link all the shaders to the final program
  glLinkProgram(program_id);

  return program_id;
}

void freeShader(int shader_id) { glDeleteProgram(shader_id); }

} // namespace

Shader::Shader(std::string name) {
  this->name = name;

  if (shader_map.count(name) != 0) {
    auto &shader_count = shader_map[name];
    shader_count.second++;
    id = shader_count.first;
    return;
  }

  debug("shader " << name << " will be compiled");

  id = compileShader(name);
  shader_map[name] = {id, 1};
}

int Shader::getShaderId(void) { return id; }

Shader::~Shader(void) {
  auto &shader_count = shader_map[name];
  if (shader_count.second-- != 1) {
    return;
  }

  debug("shader " << name << " will be deleted");

  freeShader(shader_count.first);
  shader_map.erase(name);
}
