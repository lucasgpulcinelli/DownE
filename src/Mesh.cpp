#include "Mesh.h"
#include "utils.h"

#include <fstream>
#include <limits>
#include <sstream>
#include <vector>

extern "C" {
#include <GL/glew.h>

#define STBI_NO_FAILURE_STRINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
}

using namespace engine;

namespace {

// storeFace scans a single line in a wavefront file (in stream) where the first
// chacter is a face (already read at this point) and adds the coordinates and
// texture indicies to tries, reading from verticies and textures to do that.
void storeFace(std::vector<float> &tris, std::vector<float> &verticies,
               std::vector<float> &textures, std::istringstream &stream) {
  std::vector<std::pair<int, int>> tri_indicies;
  int vi, vti, vni;
  std::string word;

  // read each num/num/num tuple
  while (stream >> word) {
    // we do not support %d//%d
    int read = std::sscanf(word.c_str(), "%d/%d/%d", &vi, &vti, &vni);
    if (read <= 0) {
      error("mesh file is incorrect");
    }
    if (read == 1) {
      error("loading meshes without textures not supported");
    }

    tri_indicies.push_back({vi - 1, vti - 1});
  }

  // for each tuple, make a triangle such that the whole line equates to a
  // polygon
  for (int i = 1; i < (int)tri_indicies.size() - 1; i++) {
    for (int j = 0; j < 3; j++) {
      tris.push_back(verticies[tri_indicies[0].first * 3 + j]);
    }
    tris.push_back(textures[tri_indicies[0].second * 2]);
    tris.push_back(textures[tri_indicies[0].second * 2 + 1]);

    for (int j = 0; j < 3; j++) {
      tris.push_back(verticies[tri_indicies[i].first * 3 + j]);
    }
    tris.push_back(textures[tri_indicies[i].second * 2]);
    tris.push_back(textures[tri_indicies[i].second * 2 + 1]);

    for (int j = 0; j < 3; j++) {
      tris.push_back(verticies[tri_indicies[i + 1].first * 3 + j]);
    }
    tris.push_back(textures[tri_indicies[i + 1].second * 2]);
    tris.push_back(textures[tri_indicies[i + 1].second * 2 + 1]);
  }
}

// readMeshTris reads all tris in a file in the wavefont obj format and return a
// vector of vertex coordinates and texels and all material name and indicies of
// the object, ordered for use with glDrawArrays in the GL_TRIANGLES mode.
std::pair<std::vector<float>, std::vector<std::pair<std::string, int>>>
readMeshTris(std::string mesh_path) {
  debug("start mesh reading for " << mesh_path);
  std::fstream f(mesh_path, std::ios_base::in);

  std::vector<float> verticies;
  std::vector<float> textures;
  std::vector<float> tris;
  std::vector<std::pair<std::string, int>> material_indicies;

  // for each line
  std::string line;
  while (std::getline(f, line)) {
    std::istringstream stream(line);

    std::string s;
    stream >> s;
    if (s == "v") {
      // read a vertex into the verticies array

      float x, y, z;
      stream >> x >> y >> z;
      verticies.push_back(x);
      verticies.push_back(y);
      verticies.push_back(z);
    } else if (s == "usemtl") {
      // read the material name and store it

      std::string texture_name;
      stream >> texture_name;

      debug("changed mtl to " << texture_name << " at vertex "
                              << tris.size() / 5);
      material_indicies.push_back({texture_name, tris.size()});
    } else if (s == "vt") {
      // read the texel coordinates and store it in the textures array

      float u, v;
      stream >> u >> v;
      textures.push_back(u);
      textures.push_back(1 - v);
    } else if (s == "f") {
      // read the face indicies and store them in the final tries array
      storeFace(tris, verticies, textures, stream);
    }
  }

  f.close();

  debug("done reading mesh, got " << tris.size() / 5 << " verticies and "
                                  << material_indicies.size() << " materials");

  return {tris, material_indicies};
}

// readMaterialFile reads a mtl file and returns all important properties: the
// map from material names to texture filenames
std::map<std::string, std::string> readMaterialFile(std::string mtl_file) {
  std::fstream f(mtl_file, std::ios_base::in);

  std::string line;

  std::map<std::string, std::string> out;
  std::string current_name;

  while (std::getline(f, line)) {
    std::istringstream stream(line);

    std::string s;
    stream >> s;
    if (s == "newmtl") {
      stream >> current_name;
    } else if (s == "map_Kd") {
      std::string filename;
      stream >> filename;
      out[current_name] = filename;
    }
  }

  return out;
}

// loadTexture creates a new opengl texture from a file and returns its id
int loadTexture(std::string texture_file) {
  int width, height, nr_channels;
  uint8_t *data =
      stbi_load(texture_file.c_str(), &width, &height, &nr_channels, 0);

  debug("data has " << nr_channels << " channels");
  if (data == nullptr) {
    error(texture_file << " returned null when tring to read");
  }

  uint32_t texture;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

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

  return texture;
}

// loadMesh creates a whole new mesh from a mesh name, using a certain
// shader and having a texture map for reusing textures between meshes.
// the method returns the newly created VAO and VBO, the mesh size, the
// relation between textures and verticies indicies, and the bounding box for
// the object.
std::tuple<int, int, int, std::vector<std::pair<int, int>>, std::vector<float>>
loadMesh(int shader_id, std::string name,
         std::map<std::string, std::pair<int, int>> &texture_id_map) {
  uint32_t vao, vbo;

  // read the verticies and material indicies
  const std::string mesh_path = "res/meshes/" + name + "/mesh.obj";
  auto points_matindicies = readMeshTris(mesh_path);

  // read the material file to get the map
  const std::string mtl_path = "res/meshes/" + name + "/mesh.mtl";
  auto texture_file_map = readMaterialFile(mtl_path);

  auto points = points_matindicies.first;
  std::vector<std::pair<int, int>> texture_indicies;

  // for each material, add a texture index entry
  for (auto mat_i : points_matindicies.second) {
    auto texture_file = texture_file_map[mat_i.first];

    // if it was already loaded before, reuse the entry
    if (texture_id_map.find(texture_file) != texture_id_map.end()) {
      auto &texid_count = texture_id_map[texture_file];
      texid_count.second++;
      texture_indicies.push_back({texid_count.first, mat_i.second});
      continue;
    }

    debug("loading texture at " << texture_file);

    // it not, load the texture from the file and add it to the map for reuse
    const std::string texture_path = "res/textures/" + texture_file;
    auto texture_id = loadTexture(texture_path);
    texture_id_map[texture_file] = {texture_id, 1};
    texture_indicies.push_back({texture_id, mat_i.second});
  }

  int size = points.size();

  // calculate the bounding box, first the min 3 coordinates then the max 3
  // coordinates
  auto bbox = std::vector<float>(6, std::numeric_limits<float>::max());
  bbox[3] = 0;
  bbox[4] = 0;
  bbox[5] = 0;

  for (int i = 0; i < size; i += 5) {
    bbox[0] = std::min(bbox[0], points[i]);
    bbox[1] = std::min(bbox[1], points[i + 1]);
    bbox[2] = std::min(bbox[2], points[i + 2]);
    bbox[3] = std::max(bbox[3], points[i]);
    bbox[4] = std::max(bbox[4], points[i + 1]);
    bbox[5] = std::max(bbox[5], points[i + 2]);
  }

  // create the vbo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // set the vbo with the data
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), points.data(),
               GL_STATIC_DRAW);

  // create the vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // set the vertex coordinates and texels with the correct data from the vbo

  GLint loc = glGetAttribLocation(shader_id, "point");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);

  loc = glGetAttribLocation(shader_id, "texture");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(sizeof(float) * 3));

  return {vao, vbo, size, texture_indicies, bbox};
}
} // namespace

// vao, vbo, mesh size, texture indicies and starting vertex, and count
std::map<std::string,
         std::tuple<int, int, int, std::vector<std::pair<int, int>>,
                    std::vector<float>, int>>
    Mesh::mesh_map;

// texture id and count
std::map<std::string, std::pair<int, int>> Mesh::texture_id_map;

Mesh::Mesh(Shader *s, std::string name) {
  debug("creating mesh for " << name);
  this->name = name;

  if (mesh_map.count(name) != 0) {
    auto &t = mesh_map[name];
    vao = std::get<0>(t);
    vbo = std::get<1>(t);
    size = std::get<2>(t);
    texture_indicies = std::get<3>(t);
    bounding_box = std::get<4>(t);
    std::get<5>(t)++;
    return;
  }

  debug("mesh " << name << " will be loaded");

  auto properties = loadMesh(s->getShaderId(), name, texture_id_map);
  vao = std::get<0>(properties);
  vbo = std::get<1>(properties);
  size = std::get<2>(properties);
  texture_indicies = std::get<3>(properties);
  bounding_box = std::get<4>(properties);

  mesh_map[name] = {vao, vbo, size, texture_indicies, bounding_box, 1};
}

int Mesh::getVAO(void) { return vao; }

const std::vector<std::pair<int, int>> &Mesh::getTextureIndicies(void) {
  return texture_indicies;
}

int Mesh::getSize(void) { return size; }

Mesh::~Mesh() {
  auto &t = mesh_map[name];

  if (std::get<5>(t)-- != 1) {
    return;
  }

  // TODO: delete textures and other opengl objects

  glDeleteVertexArrays(1, (uint32_t *)&std::get<0>(t));
  glDeleteBuffers(1, (uint32_t *)&std::get<1>(t));

  debug("mesh " << name << " will be deleted");

  mesh_map.erase(name);
}

const std::vector<float> &Mesh::getBoundingBox(void) { return bounding_box; }
