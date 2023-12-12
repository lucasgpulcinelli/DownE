#ifndef CAMERA_OBJECT_H
#define CAMERA_OBJECT_H
#include "abstracts/Object.h"

extern "C" {
#include <GL/glew.h>
#include <GLFW/glfw3.h>
}

namespace engine {
class CameraObject : public Object {
private:
  static CameraObject *single_camera;

  float view_angle[2] = {0, 0};
  float position[3] = {0, 0, 0};

public:
  CameraObject(void);
  ~CameraObject(void);

  static CameraObject *getCamera(void);

  void draw(int, int, int) override{};
  void frame(void) override;

  const float *getPosition(void) { return position; }
  const float *getViewAngle(void) { return view_angle; }

  static void mouseCallback(GLFWwindow *, double x, double y);
};
} // namespace engine

#endif
