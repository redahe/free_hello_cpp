#include "system.hpp"

#include <tuple> // For std::ignore

int main() {
  std::ignore = _system::write("Hello World\n");
  return 0;
}
