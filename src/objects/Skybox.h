#ifndef SKYBOX_H
#define SKYBOX_H

#include "Object3DWithLight.h"

namespace engine {
class Skybox : public Object3DWithLight {
private:
  static Skybox *single_skybox;

public:
  Skybox() : Object3DWithLight("simple3d", "skybox") { single_skybox = this; }
  static float getSkyboxRadius(void) {
    return std::abs(single_skybox->m->getBoundingBox()[0]);
  }
};
} // namespace engine

#endif
