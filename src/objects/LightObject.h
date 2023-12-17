#ifndef LIGHT_OBJECT_H
#define LIGHT_OBJECT_H

#include "Object3DWithLight.h"

namespace engine {
class LightObject : public Object3DWithLight {
private:
  static LightObject *single_light;
  float color[3];

public:
  LightObject(float position[3], float color[3]);
  ~LightObject(void);

  void frame(void) override;
  static const float *getPosition(void);
  static const float *getColor(void);
};
} // namespace engine

#endif
