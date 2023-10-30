#include "Engine.h"

#include "objects/abstracts/Object.h"
#include "utils.h"

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
  glActiveTexture(GL_TEXTURE0);

  glDisable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

  std::thread frame_thread([this]() {
    while (!glfwWindowShouldClose(window)) {
      auto time = glfwGetTime();

      for (auto *obj : object_set) {
        obj->frame();
      }

      auto delta = glfwGetTime() - time;
      if (delta < 1.0f / 30) {
        std::this_thread::sleep_for(
            std::chrono::duration<double>(1.0f / 30 - delta));
      }
    }
  });

  while (!glfwWindowShouldClose(window)) {
    auto time = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int curr_shader = -1;
    int curr_texture = -1;
    int curr_vao = -1;

    for (auto drawable : drawable_set) {
      if (curr_shader != std::get<0>(drawable)) {
        curr_shader = std::get<0>(drawable);
        glUseProgram(curr_shader);
      }

      if (curr_texture != std::get<1>(drawable) &&
          std::get<1>(drawable) != -1) {
        curr_texture = std::get<1>(drawable);
        glBindTexture(GL_TEXTURE_2D, curr_texture);
      }

      if (curr_vao != std::get<2>(drawable)) {
        curr_vao = std::get<2>(drawable);
        glBindVertexArray(curr_vao);
      }

      std::get<4>(drawable)->draw(curr_texture, curr_vao,
                                  std::get<3>(drawable));
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

  for (auto *obj : object_set) {
    delete obj;
  }

  frame_thread.join();
}

void Engine::addObject(Object *obj) {
  for (auto drawable : obj->getDrawables()) {
    drawable_set.insert(drawable);
  }

  object_set.insert(obj);
}

void Engine::deleteObject(Object *obj) {
  object_set.erase(obj);

  for (auto drawable : obj->getDrawables()) {
    drawable_set.erase(drawable);
  }

  delete obj;
}
