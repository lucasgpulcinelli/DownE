#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <string>
#include <vector>

namespace objects {
class Object {
public:
  Object(std::string shader_name);
  virtual ~Object(void);
  virtual void draw(int w, int h);
  const std::vector<int> getProperties(void);

protected:
  std::vector<int> properties;

private:
  std::string shader_name;

  static int shader_now;
  static std::map<std::string, std::pair<int, int>> shader_map;
};
} // namespace objects

#endif
