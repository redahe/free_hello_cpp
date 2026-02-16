#include "system.hpp"

#include <string_view> // For std::string_view_literals
#include <tuple>       // For std::ignore

using namespace std::string_view_literals;

int main() {
  std::ignore = _system::write("Hello World!\n"sv);
  return 0;
}
