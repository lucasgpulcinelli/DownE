#ifndef ENGINE_H
#define ENGINE_H

#include <map>
#include <set>
#include <vector>

extern "C" {
#include <GL/glew.h>
#include <GLFW/glfw3.h>
}

namespace objects {
class Object;
} // namespace objects

namespace engine {
class Engine {
public:
  Engine(void);
  ~Engine(void);

  static Engine *getEngine(void);
  const std::set<int> &getPressedKeys(void);
  std::pair<int, int> getWindowSize(void);

  void run(void);

  void addObject(objects::Object *obj);
  void deleteObject(objects::Object *obj);

private:
  int width, height;

  std::map<std::vector<int>, objects::Object *> objects_map;

  std::set<int> pressed_keys;
  GLFWwindow *window;

  static void inputCallback(__attribute__((unused)) GLFWwindow *w, int key,
                            int scan_code, int action, int modifiers);
  static Engine *single_engine;
};
} // namespace engine

#endif
