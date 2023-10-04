#include "Object.h"
#include "utils.h"

using namespace engine;

Object::~Object(void) {}

const std::vector<drawable_t> &Object::getDrawables(void) { return drawables; }
