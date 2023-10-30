#ifndef ENGINE_H
#define ENGINE_H

#include <functional>
#include <map>
#include <set>
#include <vector>

extern "C" {
#include <GL/glew.h>
#include <GLFW/glfw3.h>
}

namespace engine {

class Object;

// type drawable defines the entry type for the drawable_set in the engine:
// all drawables have one and only one shader, texture, VAO and an internal id,
// as well as a parent object that owns it.
// This is done that way in order to minimize shader and texture switches.
// The parent object is responsible for implementing the uniform setting and
// glDrawArrays calling.
typedef std::tuple<int, int, int, int, Object *> drawable;

// class Engine defines the main game manager as a singleton. When constructed,
// a GLFW window is opened where all the game actions happen. It manages both
// key input via a callback and the main drawing loop via the run method.
class Engine {
public:
  // the constructor creates the engine with a window the size of the primary
  // monitor screen. It also initializes GLFW and GLEW and sets the key input
  // and openGL debug callbacks.
  Engine(void);

  // the destructor closes the window, while also terminating GLFW.
  // TODO: destroy objects and stop callbacks, such that a new Engine can be
  // created afterwards without memory leaking.
  ~Engine(void);

  // getEngine gets the singleton engine instance, or nullptr if it does not
  // exist.
  static Engine *getEngine(void);

  // getPressedKeys gets the set of all currently pressed keys as GFLWKeys.
  // TODO: support modifiers.
  // TODO: profile std::set usage and determine if it is too costly in our case.
  const std::set<int> &getPressedKeys(void);

  // getWindowSize gets the size of the window when it was created, in the form
  // .first = width, .second = height.
  // TODO: support resizing via GLFW callback.
  std::pair<int, int> getWindowSize(void);

  // run initializes the main drawing loop, returning only when the GLFWWindow
  // should be closed.
  void run(void);

  // addObject adds a new Object to be managed by the main drawing loop.
  void addObject(Object *obj);

  // deleteObject removes an already managed object from the main drawing loop.
  void deleteObject(Object *obj);

private:
  // the window size.
  int width, height;

  // the monitor refresh rate, used in the drawing loop
  int refresh_rate;

  // the set of all drawables that should be drawn, ordered by the properties
  // described in the drawable type
  std::set<drawable> drawable_set;

  // the set of all objects currently managed by the engine
  std::set<Object *> object_set;

  // the set of currently pressed keys
  std::set<int> pressed_keys;

  // the main GLFW window where the game happens.
  GLFWwindow *window;

  // inputCallback populates the pressed_keys set.
  static void inputCallback(__attribute__((unused)) GLFWwindow *w, int key,
                            int scan_code, int action, int modifiers);

  // the singleton Engine pointer
  static Engine *single_engine;
};
} // namespace engine

#endif
