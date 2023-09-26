#include "Engine.h"

#include "objects/abstracts/Object.h"
#include "utils.h"

#include <filesystem>
#include <iostream>
#include <thread>

using namespace engine;

Engine *Engine::single_engine = nullptr;

namespace {
void messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                     __attribute__((unused)) GLsizei len, const GLchar *message,
                     __attribute__((unused)) const void *user_param) {
  if (type == GL_DEBUG_TYPE_ERROR) {
    error("opengl error: " << source << id << severity << " " << message);
  } else {
    debug("opengl debug:" << source << id << severity << " " << message);
  }
}
} // namespace

void Engine::inputCallback(__attribute__((unused)) GLFWwindow *w, int key,
                           int scan_code, int action, int modifiers) {
  debug("key pressed: " << key << " " << scan_code << " " << action << " "
                        << modifiers);

  if (action == GLFW_PRESS) {
    getEngine()->pressed_keys.insert(key);
  } else if (action == GLFW_RELEASE) {
    getEngine()->pressed_keys.erase(key);
  }
}

Engine::Engine(void) {
  if (Engine::single_engine != nullptr) {
    error("creating an engine when other already exists");
  }

  Engine::single_engine = this;

  info("initilizing engine");
  if (!glfwInit()) {
    error("failed to start GLFW");
  }

  auto *monitor = glfwGetPrimaryMonitor();
  if (monitor == nullptr) {
    error("failed to get primary monitor");
  }

  const auto *mode = glfwGetVideoMode(monitor);
  this->width = mode->width;
  this->height = mode->height;
  this->refresh_rate = mode->refreshRate;

  debug("creating window");
  window = glfwCreateWindow(mode->width, mode->height, "main engine", monitor,
                            nullptr);

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    error("failed to start GLEW");
  }

  glfwSetKeyCallback(window, Engine::inputCallback);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(messageCallback, 0);

  glEnable(GL_DEPTH_TEST);

  info("engine initalized");
}

Engine::~Engine(void) {
  Engine::single_engine = nullptr;
  info("stopping engine");

  glfwDestroyWindow(window);
  glfwTerminate();

  info("bye!");
}

const std::set<int> &Engine::getPressedKeys(void) { return pressed_keys; }

std::pair<int, int> Engine::getWindowSize(void) { return {width, height}; }

Engine *Engine::getEngine(void) { return single_engine; }

void Engine::run(void) {
  info("starting main loop");

  while (!glfwWindowShouldClose(window)) {
    auto time = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto vec_objs : objects_map) {
      vec_objs.second->draw(width, height);
    }

    glfwPollEvents();

    // framerate cap at the monitor refresh rate
    // TODO: this should be configurable
    auto delta = glfwGetTime() - time;
    if (delta < 1.0f / refresh_rate) {
      std::this_thread::sleep_for(
          std::chrono::duration<double>(1.0f / refresh_rate - delta));
    }

    glfwSwapBuffers(window);
  }
}

void Engine::addObject(objects::Object *obj) {
  auto &v = obj->getProperties();
  objects_map[v] = obj;
}

void Engine::deleteObject(objects::Object *obj) {
  auto v = obj->getProperties();
  objects_map.erase(v);

  delete obj;
}
